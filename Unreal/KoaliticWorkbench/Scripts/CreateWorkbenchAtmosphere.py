"""Build a separate UI-domain atmosphere material for the application chrome."""
from pathlib import Path
import unreal

lib = unreal.MaterialEditingLibrary
asset_path = '/Game/InterfaceMaterials/M_WorkbenchAtmosphere'
material = unreal.load_asset(asset_path)
if not material:
    material = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        'M_WorkbenchAtmosphere', '/Game/InterfaceMaterials', unreal.Material,
        unreal.MaterialFactoryNew())
lib.delete_all_material_expressions(material)
material.set_editor_property('material_domain', unreal.MaterialDomain.MD_UI)
material.set_editor_property('blend_mode', unreal.BlendMode.BLEND_TRANSLUCENT)
custom = lib.create_material_expression(material, unreal.MaterialExpressionCustom, 0, 0)
custom.set_editor_property('code', Path(__file__).with_name('WorkbenchAtmosphere.hlsl').read_text())
custom.set_editor_property('output_type', unreal.CustomMaterialOutputType.CMOT_FLOAT4)
inputs = []
for name in ('UV', 'Clock'):
    item = unreal.CustomInput()
    item.set_editor_property('input_name', name)
    inputs.append(item)
custom.set_editor_property('inputs', inputs)
uv = lib.create_material_expression(material, unreal.MaterialExpressionTextureCoordinate, -450, 0)
clock = lib.create_material_expression(material, unreal.MaterialExpressionScalarParameter, -450, 180)
clock.set_editor_property('parameter_name', 'Clock')
lib.connect_material_expressions(uv, '', custom, 'UV')
lib.connect_material_expressions(clock, '', custom, 'Clock')
rgb = lib.create_material_expression(material, unreal.MaterialExpressionComponentMask, 350, 0)
for channel in ('r', 'g', 'b'):
    rgb.set_editor_property(channel, True)
rgb.set_editor_property('a', False)
alpha = lib.create_material_expression(material, unreal.MaterialExpressionComponentMask, 350, 160)
for channel in ('r', 'g', 'b'):
    alpha.set_editor_property(channel, False)
alpha.set_editor_property('a', True)
lib.connect_material_expressions(custom, '', rgb, '')
lib.connect_material_expressions(custom, '', alpha, '')
lib.connect_material_property(rgb, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
lib.connect_material_property(alpha, '', unreal.MaterialProperty.MP_OPACITY)
lib.recompile_material(material)
unreal.EditorAssetLibrary.save_loaded_asset(material)
unreal.log('KOALITIC_WORKBENCH_ATMOSPHERE_CREATED')
