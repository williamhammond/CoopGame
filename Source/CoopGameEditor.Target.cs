// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class CoopGameEditorTarget : TargetRules
{
	public CoopGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange(new[] { "CoopGame" });
	}
}