﻿/*
* Copyright (c) <2023> Side Effects Software Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*
* 2. The name of Side Effects Software may not be used to endorse or
*    promote products derived from this software without specific prior
*    written permission.
*
* THIS SOFTWARE IS PROVIDED BY SIDE EFFECTS SOFTWARE "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
* NO EVENT SHALL SIDE EFFECTS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "CoreMinimal.h"
#include "HoudiniEngineRuntimeCommon.h"
#include "HoudiniInput.h"

#include "HoudiniPreset.generated.h"

/**
 * This is a Houdini Tools package descriptor inside of UE, typically created
 * after a HoudiniToolsPackage has been imported into the UE project.
 */

class UHoudiniInput;
class UHoudiniAsset;
class UHoudiniParameterChoice;
class UHoudiniParameterColor;
class UHoudiniParameterToggle;
class UHoudiniParameterInt;
class UHoudiniParameterFloat;
class UHoudiniParameterString;
class UHoudiniParameterFile;
class UHoudiniParameterRampFloat;
class UHoudiniParameterRampColor;
class UHoudiniParameterOperatorPath;


UENUM()
enum class EHoudiniPresetValueType
{
	Invalid,
	Float,
	Int,
	String
};


USTRUCT()
struct FHoudiniPresetBase
{
	GENERATED_BODY()
	virtual ~FHoudiniPresetBase() {}

	virtual FString ToString() { return FString(); }

	virtual EHoudiniPresetValueType GetValueType() { return EHoudiniPresetValueType::Invalid; }
};


USTRUCT(BlueprintType)
struct FHoudiniPresetFloatValues : public FHoudiniPresetBase
{
	GENERATED_BODY()
	
	virtual EHoudiniPresetValueType GetValueType() override { return EHoudiniPresetValueType::Float; }

	virtual FString ToString() override { return FString::JoinBy(Values, TEXT(", "), [](const float& Value) { return FString::SanitizeFloat(Value); }); }
	
	UPROPERTY(EditAnywhere)
	TArray<float> Values;
};


USTRUCT(BlueprintType)
struct FHoudiniPresetIntValues : public FHoudiniPresetBase
{
	GENERATED_BODY()

	virtual EHoudiniPresetValueType GetValueType() override { return EHoudiniPresetValueType::Int; }

	virtual FString ToString() override { return FString::JoinBy(Values, TEXT(", "), [](const float& Value) { return FString::FromInt(Value); }); }
	
	UPROPERTY(EditAnywhere)
	TArray<int> Values;
};


USTRUCT(BlueprintType)
struct FHoudiniPresetStringValues : public FHoudiniPresetBase
{
	GENERATED_BODY()

	virtual EHoudiniPresetValueType GetValueType() override { return EHoudiniPresetValueType::String; }

	virtual FString ToString() override { return FString::Join(Values, TEXT(", ")); }
	
	UPROPERTY(EditAnywhere)
	TArray<FString> Values;
};

USTRUCT(BlueprintType)
struct FHoudiniPresetRampFloatPoint
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Position;
	
	UPROPERTY(EditAnywhere)
	float Value;

	UPROPERTY(EditAnywhere)
	EHoudiniRampInterpolationType Interpolation;
};

USTRUCT(BlueprintType)
struct FHoudiniPresetRampColorPoint
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Position;
	
	UPROPERTY(EditAnywhere)
	FLinearColor Value;

	UPROPERTY(EditAnywhere)
	EHoudiniRampInterpolationType Interpolation;
};

USTRUCT(BlueprintType)
struct FHoudiniPresetRampFloatValues : public FHoudiniPresetBase
{
	GENERATED_BODY()

	virtual EHoudiniPresetValueType GetValueType() override { return EHoudiniPresetValueType::String; }

	virtual FString ToString() override { return FString::Format(TEXT("%d float points."), { Points.Num() }); }

	UPROPERTY(EditAnywhere)
	TArray<FHoudiniPresetRampFloatPoint> Points;
};

USTRUCT(BlueprintType)
struct FHoudiniPresetRampColorValues : public FHoudiniPresetBase
{
	GENERATED_BODY()

	virtual EHoudiniPresetValueType GetValueType() override { return EHoudiniPresetValueType::String; }

	virtual FString ToString() override { return FString::Format(TEXT("%d color points."), { Points.Num() }); }

	UPROPERTY(EditAnywhere)
	TArray<FHoudiniPresetRampColorPoint> Points;
};

USTRUCT(BlueprintType)
struct FHoudiniPresetInputObject : public FHoudiniPresetBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UObject> InputObject;
	
	UPROPERTY(EditAnywhere)
	FTransform Transform;
};


USTRUCT(BlueprintType)
struct FHoudiniPresetInputValue : public FHoudiniPresetBase
{
	GENERATED_BODY()

	virtual EHoudiniPresetValueType GetValueType() override { return EHoudiniPresetValueType::String; }

	virtual FString ToString() override { return FString(); }

	// Export Options
	
	UPROPERTY(EditAnywhere)
	bool bKeepWorldTransform;

	UPROPERTY(EditAnywhere)
	bool bPackGeometryBeforeMerging;
	
	UPROPERTY(EditAnywhere)
	bool bExportInputAsReference;

	UPROPERTY(EditAnywhere)
	bool bExportLODs;

	UPROPERTY(EditAnywhere)
	bool bExportSockets;

	UPROPERTY(EditAnywhere)
	bool bExportColliders;

	UPROPERTY(EditAnywhere)
	bool bExportMaterialParameters;

	UPROPERTY(EditAnywhere)
	bool bMergeSplineMeshComponents;

	UPROPERTY(EditAnywhere)
	bool bPreferNaniteFallbackMesh;
	
	// Input properties

	UPROPERTY(EditAnywhere)
	EHoudiniInputType InputType;

	UPROPERTY(EditAnywhere)
	bool bIsParameterInput;

	UPROPERTY(EditAnywhere)
	FString ParameterName;

	UPROPERTY(EditAnywhere)
	int32 InputIndex;

	UPROPERTY(EditAnywhere)
	TArray<FHoudiniPresetInputObject> InputObjects;
};


struct FHoudiniPresetHelpers
{
	static bool IsSupportedInputType(const EHoudiniInputType InputType);
	
	// Generic version to populate our Preset value structs from Houdini Parameters.
	static void PopulateFromParameter(UHoudiniParameterFloat* Parm, FHoudiniPresetFloatValues& Value);
	static void PopulateFromParameter(UHoudiniParameterInt* Parm, FHoudiniPresetIntValues& Value);
	static void PopulateFromParameter(UHoudiniParameterString* Parm, FHoudiniPresetStringValues& Value);

	// Ingest int parameters
	static bool IngestParameter(UHoudiniParameterInt* Parm, TMap<FString,FHoudiniPresetIntValues>& OutValues, FString& OutValueStr);
	static bool IngestParameter(UHoudiniParameterChoice* Parm, TMap<FString,FHoudiniPresetIntValues>& OutValues, FString& OutValueStr);
	static bool IngestParameter(UHoudiniParameterToggle* Parm, TMap<FString,FHoudiniPresetIntValues>& OutValues, FString& OutValueStr);

	// Ingest float parameters
	static bool IngestParameter(UHoudiniParameterFloat* Parm, TMap<FString,FHoudiniPresetFloatValues>& OutValues, FString& OutValueStr);
	static bool IngestParameter(UHoudiniParameterColor* Parm, TMap<FString,FHoudiniPresetFloatValues>& OutValues, FString& OutValueStr);

	// Ingest string parameters
	static bool IngestParameter(UHoudiniParameterString* Parm, TMap<FString,FHoudiniPresetStringValues>& OutValues, FString& OutValueStr);
	static bool IngestParameter(UHoudiniParameterFile* Parm, TMap<FString,FHoudiniPresetStringValues>& OutValues, FString& OutValueStr);
	static bool IngestParameter(UHoudiniParameterChoice* Parm, TMap<FString,FHoudiniPresetStringValues>& OutValues, FString& OutValueStr);

	// Ingest ramp parameters
	static bool IngestParameter(UHoudiniParameterRampFloat* Parm, TMap<FString,FHoudiniPresetRampFloatValues>& OutValues, FString& OutValueStr);
	static bool IngestParameter(UHoudiniParameterRampColor* Parm, TMap<FString,FHoudiniPresetRampColorValues>& OutValues, FString& OutValueStr);

	// Ingest input parameters
	// static bool IngestParameterInput(UHoudiniParameterOperatorPath* Parm, TArray<FHoudiniPresetInputValue>& OutValues, FString& OutValueStr);
	// static bool IngestAbsoluteInput(int32 InputIndex, UHoudiniInput* Parm, TArray<FHoudiniPresetInputValue>& OutValues, FString& OutValueStr);
	static void IngestGenericInput(UHoudiniInput* Input, bool bIsParameterInput, const FString& ParameterName, TArray<FHoudiniPresetInputValue>& OutValues, FString& OutValueStr);
	static void UpdateFromInput(FHoudiniPresetInputValue& Value, UHoudiniInput* Input);
	

	// Preset Helpers
	
	// Int based parameters
	static void ApplyPresetParameterValues(const FHoudiniPresetIntValues& Values, UHoudiniParameterInt* Param);
	static void ApplyPresetParameterValues(const FHoudiniPresetIntValues& Values, UHoudiniParameterChoice* Param);
	static void ApplyPresetParameterValues(const FHoudiniPresetIntValues& Values, UHoudiniParameterToggle* Param);

	// Float based parameters
	static void ApplyPresetParameterValues(const FHoudiniPresetFloatValues& Values, UHoudiniParameterFloat* Param);
	static void ApplyPresetParameterValues(const FHoudiniPresetFloatValues& Values, UHoudiniParameterColor* Param);

	// String based parameters
	static void ApplyPresetParameterValues(const FHoudiniPresetStringValues& Values, UHoudiniParameterString* Param);
	static void ApplyPresetParameterValues(const FHoudiniPresetStringValues& Values, UHoudiniParameterFile* Param);
	static void ApplyPresetParameterValues(const FHoudiniPresetStringValues& Values, UHoudiniParameterChoice* Param);

	// Ramp based parameters
	static void ApplyPresetParameterValues(const FHoudiniPresetRampFloatValues& Values, UHoudiniParameterRampFloat* Param);
	static void ApplyPresetParameterValues(const FHoudiniPresetRampColorValues& Values, UHoudiniParameterRampColor* Param);

	// Input parameters
	static void ApplyPresetParameterValues(const FHoudiniPresetInputValue& Values, UHoudiniInput* Param);
};


UCLASS(BlueprintType, hidecategories=(Object))
class HOUDINIENGINEEDITOR_API UHoudiniPreset : public UObject
{
	GENERATED_BODY()
public:

	UHoudiniPreset();

	// The label for this preset
	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	FString Name;

	// A description for this preset
	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	FString Description;

	// The HoudiniAsset linked to this preset.
	// Should this be a soft object pointer instead?
	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	UHoudiniAsset* SourceHoudiniAsset;
	
	// Whether the revert all parameters on the HDA to their default values before applying this preset
	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	bool bRevertHDAParameters;

	// Whether this preset can be applied to any HDA, or only the SourceHoudiniAsset.
	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	bool bApplyOnlyToSource;

	// Whether this preset be instantiated (using the SourceHoudiniAsset).
	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	bool bCanInstantiate;

	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	TMap<FString, FHoudiniPresetFloatValues> FloatParameters;

	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	TMap<FString, FHoudiniPresetIntValues> IntParameters;

	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	TMap<FString, FHoudiniPresetStringValues> StringParameters;

	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	TMap<FString, FHoudiniPresetRampFloatValues> RampFloatParameters;
	
	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	TMap<FString, FHoudiniPresetRampColorValues> RampColorParameters;

	// Track parameter-based / absolute inputs
	UPROPERTY(EditAnywhere, Category="Houdini Preset")
	TArray<FHoudiniPresetInputValue> InputParameters;
};