#pragma once

#include "WorkbenchActions.h"

namespace KW {
enum class EUiCue : int8 { None=-1, Navigate, Select, Confirm, Enter, Exit, Reward, Count };

inline EUiCue CueForAction(int Id){
 if(Id==ActionId::Close)return EUiCue::None;
 if(Id==ActionId::ReplayEnter)return EUiCue::Enter;
 if(Id==ActionId::PreviewExit)return EUiCue::Exit;
 if(Id==ActionId::Reward50||Id==ActionId::Reward100||Id==ActionId::Reward150||Id==ActionId::Pulse)return EUiCue::Reward;
 if(Id==ActionId::SaveComponent||Id==ActionId::SavePreset||Id==ActionId::ApplyPreset||Id==ActionId::ReloadComponent||Id==ActionId::ReloadPreset||Id==ActionId::DeletePreset)return EUiCue::Confirm;
 if(Id==ActionId::GoHome||Id==ActionId::GoWorkbench||Id==ActionId::GoEpisodes||Id==ActionId::GoEditor||Id==ActionId::OpenComponents||Id==ActionId::OpenKit||Id==ActionId::InspectorContent||Id==ActionId::InspectorMotion||Id==ActionId::InspectorStyle||Id==ActionId::SelectProfile||Id==ActionId::ShowLayers||Id==ActionId::OpenProgressPart||Id==ActionId::ComponentSearch||Id==ActionId::ToggleDrawer||(Id>=ActionId::SelectLayerBase&&Id<ActionId::SelectLayerBase+8)||
    (Id>=ActionId::CategoryBase&&Id<ActionId::CategoryBase+7)||(Id>=ActionId::SectionBase&&Id<ActionId::SectionBase+7)||Id==ActionId::EventSection||Id==ActionId::OpenDesign||Id==ActionId::BackToProfile||Id==ActionId::PreviousControls||Id==ActionId::NextControls||
    Id==ActionId::PreviousPresetPage||Id==ActionId::NextPresetPage||Id==ActionId::PreviousPickerPage||Id==ActionId::NextPickerPage||Id==ActionId::Expanded||Id==ActionId::Compact||Id==ActionId::BarSolo||Id==ActionId::PreviousVariant||Id==ActionId::NextVariant||
    (Id>=ActionId::LinkCategoryBase&&Id<ActionId::LinkCategoryBase+7))
    return EUiCue::Navigate;
 if(Id==ActionId::ToggleSound||Id==ActionId::TogglePresetPicker||Id==ActionId::DismissPresetPicker||Id==ActionId::CreateVariant||Id==ActionId::Undo||Id==ActionId::Redo||Id==ActionId::PausePreview||Id==ActionId::StopPreview||Id==ActionId::ResetTest||
    Id==ActionId::Pin||Id==ActionId::Disabled||Id==ActionId::AutoCollapse||Id==ActionId::NearThreshold||Id==ActionId::ClearRewards||
    Id==ActionId::Portrait||Id==ActionId::ResetCategory||Id==ActionId::SaveAs||
    (Id>=ActionId::ProfileName&&Id<=ActionId::VariantName)||
    (Id>=ActionId::LayerBase&&Id<ActionId::LayerBase+8)||
    (Id>=ActionId::ParameterResetBase&&Id<ActionId::ParameterResetBase+100)||
    (Id>=ActionId::PresetSlotBase&&Id<ActionId::PresetSlotBase+100)||
    (Id>=ActionId::ParameterChoiceBase&&Id<ActionId::ParameterChoiceBase+100)||
    (Id>=ActionId::ParameterValueBase&&Id<ActionId::ParameterValueBase+100)||
    (Id>=ActionId::ParameterToggleBase&&Id<ActionId::ParameterToggleBase+100))return EUiCue::Select;
 return EUiCue::None;
}
}
