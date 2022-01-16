// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class CoopGameTarget : TargetRules
{
	public CoopGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange(new[] { "CoopGame" });
	}
}