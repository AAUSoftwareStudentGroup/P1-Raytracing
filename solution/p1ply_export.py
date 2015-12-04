# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

# <pep8-80 compliant>

bl_info = {
    "name": "Stanford PLY format - modified for AAU P1 group",
    "author": "Anton Christensen",
    "blender": (2, 71, 0),
    "location": "File > Export",
    "description": "Export PLY mesh data",
    "warning": "",
    "category": "Export"}

if "bpy" in locals():
    import imp
    if "export_ply" in locals():
        imp.reload(export_ply)


import os
import bmesh
import bpy
from bpy.props import (CollectionProperty,
                       StringProperty,
                       BoolProperty,
                       EnumProperty,
                       FloatProperty,
                       )
from bpy_extras.io_utils import (ExportHelper,
                                 axis_conversion,
                                 )

def tri_mesh(mesh):
    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(mesh)
    bm.free()
    del bm

def save_mesh(filepath,
              meshes,
              lights,
              camera,
              use_normals=True,
              use_uv_coords=True,
              use_colors=True,
              ):

    def rvec3d(v):
        return round(v[0], 6), round(v[1], 6), round(v[2], 6)

    def rvec2d(v):
        return round(v[0], 6), round(v[1], 6)

    file = open(filepath, "w", encoding="utf8", newline="\n")
    fw = file.write

    # Be sure tessface & co are available!
    # if not mesh.tessfaces and mesh.polygons:
        # mesh.calc_tessface()
    for m in meshes:
        m.calc_tessface()

    # has_uv = bool(mesh.tessface_uv_textures)
    # has_vcol = bool(mesh.tessface_vertex_colors)

    # if not has_uv:
        # use_uv_coords = False
    # if not has_vcol:
        # use_colors = False

    # if not use_uv_coords:
        # has_uv = False
    # if not use_colors:
        # has_vcol = False

    # if has_uv:
        # active_uv_layer = mesh.tessface_uv_textures.active
        # if not active_uv_layer:
            # use_uv_coords = False
            # has_uv = False
        # else:
            # active_uv_layer = active_uv_layer.data

    # if has_vcol:
        # active_col_layer = mesh.tessface_vertex_colors.active
        # if not active_col_layer:
            # use_colors = False
            # has_vcol = False
        # else:
            # active_col_layer = active_col_layer.data

    # in case
    normal = normal_key = None

    mesh_verts = []
    mesh_tessfaces = []
    for m in meshes:
        for v in m.vertices:
            mesh_verts.append(v)
        for t in m.tessfaces:
            mesh_tessfaces.append(t)

    # mesh_verts = mesh.vertices  # save a lookup
    ply_verts = []  # list of dictionaries
    # vdict = {} # (index, normal, uv) -> new index
    vdict = [{} for i in range(len(mesh_verts))]
    ply_faces = [[] for f in range(len(mesh_tessfaces))]
    vert_count = 0
    for i, f in enumerate(mesh_tessfaces):

        smooth = not use_normals or f.use_smooth
        if not smooth:
            normal = f.normal[:]
            normal_key = rvec3d(normal)

        # if has_uv:
            # uv = active_uv_layer[i]
            # uv = uv.uv1, uv.uv2, uv.uv3, uv.uv4
        # if has_vcol:
            # col = active_col_layer[i]
            # col = col.color1[:], col.color2[:], col.color3[:], col.color4[:]

        f_verts = f.vertices

        pf = ply_faces[i]
        for j, vidx in enumerate(f_verts):
            v = mesh_verts[vidx]

            if smooth:
                normal = v.normal[:]
                normal_key = rvec3d(normal)

            # if has_uv:
                # uvcoord = uv[j][0], uv[j][1]
                # uvcoord_key = rvec2d(uvcoord)

            # if has_vcol:
                # color = col[j]
                # color = (int(color[0] * 255.0),
                         # int(color[1] * 255.0),
                         # int(color[2] * 255.0),
                         # )
            key = normal_key#, uvcoord_key, color

            vdict_local = vdict[vidx]
            pf_vidx = vdict_local.get(key)  # Will be None initially

            if pf_vidx is None:  # same as vdict_local.has_key(key)
                pf_vidx = vdict_local[key] = vert_count
                ply_verts.append((vidx, normal))
                vert_count += 1

            pf.append(pf_vidx)

    fw("ply\n")
    fw("format ascii 1.0\n")
    fw("comment Created by Blender %s - "
       "www.blender.org, source file: %r\n" %
       (bpy.app.version_string, os.path.basename(bpy.data.filepath)))

    fw("element vertex %d\n" % len(mesh_verts))

    fw("property float x\n"
       "property float y\n"
       "property float z\n")

    if use_normals:
        fw("property float nx\n"
           "property float ny\n"
           "property float nz\n")
    # if use_uv_coords:
    #     fw("property float s\n"
    #        "property float t\n")
    # if use_colors:
    #     fw("property uchar red\n"
    #        "property uchar green\n"
    #        "property uchar blue\n")

    fw("element face %d\n" % len(mesh_tessfaces))
    fw("property list uchar uint vertex_indices\n")
    fw("element object %d\n" % len(meshes))
    fw("property uint verticies\n")
    fw("property uint triangles\n")
    fw("property uchar red\n"
       "property uchar green\n"
       "property uchar blue\n")
    fw("property float diffuse_coefficient\n")
    fw("property float specular_coefficient\n")
    fw("property float specular_hardness\n")
    fw("element light %d\n" % len(lights))
    fw("property float x\n")
    fw("property float y\n")
    fw("property float z\n")
    fw("property float intensity\n")
    fw("property uchar red\n"
       "property uchar green\n"
       "property uchar blue\n"
       "property float radius\n"
       "property uint sample_size\n")
    fw("element camera 1\n"
       "property float x\n"
       "property float y\n"
       "property float z\n")
    fw("end_header\n")

    # for i, v in enumerate(ply_verts):
    #     fw("%.6f %.6f %.6f" % mesh_verts[v[0]].co[:])  # co
    #     if use_normals:
    #         fw(" %.6f %.6f %.6f" % v[1])  # no
    #     # if use_uv_coords:
    #         # fw(" %.6f %.6f" % v[2])  # uv
    #     # if use_colors:
    #         # fw(" %u %u %u" % v[3])  # col
    #     fw("\n")

    for v in mesh_verts:
        fw("%.6f %.6f %.6f" % v.co[:])  # co
        if use_normals:
            fw(" %.6f %.6f %.6f" % v.normal[:])  # no
        fw("\n")

    # for pf in ply_faces:
    #     if len(pf) == 3:
    #         fw("3 %d %d %d\n" % tuple(pf))
    #     else:
    #         fw("4 %d %d %d %d\n" % tuple(pf))

    starting_index = 0
    higest_index = 0
    for m in meshes:    
        for p in m.polygons:
            poly_verts = [x+starting_index for x in p.vertices]
            
            fw("3")
            for i in poly_verts:
                if i > higest_index:
                  higest_index = i
                fw(" %d" % i)
            fw("\n");
        starting_index = higest_index+1

    for m in meshes:
        # fw("%s\n" % m.name)
        # fw("number of polygons - list of polygon indexes\n")
        fw("%d " % len(m.vertices))
        fw("%d " % len(m.polygons))
        fw("%d "  % int(m.materials[0].diffuse_color[0] * 255.0 +0.5))
        fw("%d "  % int(m.materials[0].diffuse_color[1] * 255.0 +0.5))
        fw("%d "  % int(m.materials[0].diffuse_color[2] * 255.0 +0.5))
        fw("%.6f "  % m.materials[0].diffuse_intensity)
        fw("%.6f "  % m.materials[0].specular_intensity)
        fw("%d\n" % m.materials[0].specular_hardness)

    for l in lights:
        fw("%.6f %.6f %.6f " % l.location[:])
        fw("%.6f " % l.data.energy)
        fw("%d "  % int(l.data.color[0] * 255.0 +0.5))
        fw("%d "  % int(l.data.color[1] * 255.0 +0.5))
        fw("%d "  % int(l.data.color[2] * 255.0 +0.5))
        fw("%.6f "  % l.data.shadow_soft_size)
        fw("%d\n"  % l.data.shadow_ray_samples)
        
    fw("%.6f %.6f %.6f\n" % camera[0].location[:])

    file.close()
    print("writing %r done" % filepath)

    return {'FINISHED'}


def save(operator,
         context,
         filepath="",
         use_mesh_modifiers=True,
         use_normals=True,
         use_uv_coords=True,
         use_colors=True,
         global_matrix=None,
         global_scale=1
         ):

    # scene = context.scene
    # obj = context.active_object
    # scene = bpy.data.scenes[0]
    # meshes = bpy.data.meshes
    objs = []
    meshes = []
    lights = []
    camera = []
    # for o in bpy.data.meshes:
        # if o.users > 0 :
            # tri_mesh(o)
            # meshes.append(o)


    if global_matrix is None:
        from mathutils import Matrix
        global_matrix = Matrix()

    if bpy.ops.object.mode_set.poll():
        bpy.ops.object.mode_set(mode='OBJECT')

    for o in bpy.data.objects:
        if o.type == 'MESH':
            objs.append(o)
        elif o.type == 'CAMERA':
            camera.append(o)
  
    for l in bpy.data.objects:
        if l.users > 0 and l.type == 'LAMP' and l.data.type == 'POINT':
            l.location *= global_scale
            lights.append(l)

    for o in objs:
        if use_mesh_modifiers and o.modifiers:
            mesh = o.to_mesh(bpy.context.scene, True, 'PREVIEW')
        else:
            mesh = o.data.copy()

        if not mesh:
            raise Exception("Error, could not get mesh data from a object")

        mesh.transform(global_matrix * o.matrix_world)
        if use_normals:
            mesh.calc_normals()

        if o.users > 0 and len(mesh.materials) > 0:
            tri_mesh(mesh)
            meshes.append(mesh)

    ret = save_mesh(filepath, meshes, lights, camera,
                    use_normals=use_normals,
                    use_uv_coords=use_uv_coords,
                    use_colors=use_colors,
                    )

    for l in lights:
        l.location *= (1/global_scale)

    # for m in meshes:
        # if use_mesh_modifiers:
            # bpy.data.meshes.remove(mesh)

    return ret


class ExportP1PLY(bpy.types.Operator, ExportHelper):
    """Export a single object as a Stanford PLY with normals, """ \
    """colors and texture coordinates"""
    bl_idname = "export_mesh_p1.ply"
    bl_label = "Export PLY"

    filename_ext = ".ply"
    filter_glob = StringProperty(default="*.ply", options={'HIDDEN'})

    use_mesh_modifiers = BoolProperty(
            name="Apply Modifiers",
            description="Apply Modifiers to the exported mesh",
            default=True,
            )
    use_normals = BoolProperty(
            name="Normals",
            description="Export Normals for smooth and "
                        "hard shaded faces "
                        "(hard shaded faces will be exported "
                        "as individual faces)",
            default=True,
            )
    # use_uv_coords = BoolProperty(
    #         name="UVs",
    #         description="Export the active UV layer",
    #         default=True,
    #         )
    # use_colors = BoolProperty(
    #         name="Vertex Colors",
    #         description="Export the active vertex color layer",
    #         default=True,
    #         )

    axis_forward = EnumProperty(
            name="Forward",
            items=(('X', "X Forward", ""),
                   ('Y', "Y Forward", ""),
                   ('Z', "Z Forward", ""),
                   ('-X', "-X Forward", ""),
                   ('-Y', "-Y Forward", ""),
                   ('-Z', "-Z Forward", ""),
                   ),
            default='Y',
            )
    axis_up = EnumProperty(
            name="Up",
            items=(('X', "X Up", ""),
                   ('Y', "Y Up", ""),
                   ('Z', "Z Up", ""),
                   ('-X', "-X Up", ""),
                   ('-Y', "-Y Up", ""),
                   ('-Z', "-Z Up", ""),
                   ),
            default='Z',
            )
    global_scale = FloatProperty(
            name="Scale",
            min=0.01, max=1000.0,
            default=1.0,
            )

    @classmethod
    def poll(cls, context):
        return context.active_object != None

    def execute(self, context):
        # from . import export_ply

        from mathutils import Matrix

        keywords = self.as_keywords(ignore=("axis_forward",
                                            "axis_up",
                                            "check_existing",
                                            "filter_glob",
                                            ))
        global_matrix = axis_conversion(to_forward=self.axis_forward,
                                        to_up=self.axis_up,
                                        ).to_4x4() * Matrix.Scale(self.global_scale, 4)
        keywords["global_matrix"] = global_matrix
        keywords["global_scale"] = self.global_scale

        filepath = self.filepath
        filepath = bpy.path.ensure_ext(filepath, self.filename_ext)

        return save(self, context, **keywords)

    def draw(self, context):
        layout = self.layout

        row = layout.row()
        row.prop(self, "use_mesh_modifiers")
        row.prop(self, "use_normals")
        # row = layout.row()
        # row.prop(self, "use_uv_coords")
        # row.prop(self, "use_colors")

        layout.prop(self, "axis_forward")
        layout.prop(self, "axis_up")
        layout.prop(self, "global_scale")


def menu_func_export(self, context):
    self.layout.operator(ExportP1PLY.bl_idname, text="AAU - Stanford (.ply)")


def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func_export)




if __name__ == "__main__":
    register()


