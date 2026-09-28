"""Run with UnrealEditor-Cmd -run=pythonscript -script=...; reproducible UI material."""
import unreal
from pathlib import Path

lib = unreal.MaterialEditingLibrary
path = '/Game/InterfaceMaterials/M_ProfileGlass'
material = unreal.load_asset(path)
if not material:
    material = unreal.AssetToolsHelpers.get_asset_tools().create_asset('M_ProfileGlass', '/Game/InterfaceMaterials', unreal.Material, unreal.MaterialFactoryNew())
lib.delete_all_material_expressions(material)
material.set_editor_property('material_domain', unreal.MaterialDomain.MD_UI)
material.set_editor_property('blend_mode', unreal.BlendMode.BLEND_TRANSLUCENT)
custom = lib.create_material_expression(material, unreal.MaterialExpressionCustom, 0, 0)
custom.set_editor_property('code', Path(__file__).with_name('ProfileGlass.hlsl').read_text())
custom.set_editor_property('output_type', unreal.CustomMaterialOutputType.CMOT_FLOAT4)
names = ['UV', 'Clock', 'Strength', 'Speed', 'Sweep', 'Particles', 'Glow', 'Reaction', 'Compact', 'Mode', 'Scan', 'Design', 'ParticleSize', 'ParticleDrift', 'Perimeter']
inputs = []
for name in names:
    entry = unreal.CustomInput()
    entry.set_editor_property('input_name', name)
    inputs.append(entry)
custom.set_editor_property('inputs', inputs)
uv = lib.create_material_expression(material, unreal.MaterialExpressionTextureCoordinate, -500, 0)
lib.connect_material_expressions(uv, '', custom, 'UV')
for i, name in enumerate(names[1:]):
    param = lib.create_material_expression(material, unreal.MaterialExpressionScalarParameter, -500, 100 + i*80)
    param.set_editor_property('parameter_name', name)
    param.set_editor_property('default_value', {'Strength': .7, 'Speed': 1., 'Sweep': .7, 'Particles': .6, 'Glow': .9}.get(name, 0.))
    lib.connect_material_expressions(param, '', custom, name)
rgb = lib.create_material_expression(material, unreal.MaterialExpressionComponentMask, 400, 0)
rgb.set_editor_property('r', True)
rgb.set_editor_property('g', True)
rgb.set_editor_property('b', True)
rgb.set_editor_property('a', False)
alpha = lib.create_material_expression(material, unreal.MaterialExpressionComponentMask, 400, 160)
alpha.set_editor_property('r', False)
alpha.set_editor_property('g', False)
alpha.set_editor_property('b', False)
alpha.set_editor_property('a', True)
lib.connect_material_expressions(custom, '', rgb, '')
lib.connect_material_expressions(custom, '', alpha, '')
lib.connect_material_property(rgb, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
lib.connect_material_property(alpha, '', unreal.MaterialProperty.MP_OPACITY)
lib.recompile_material(material)
unreal.EditorAssetLibrary.save_loaded_asset(material)
unreal.log('KOALITIC_UI_MATERIAL_CREATED')
