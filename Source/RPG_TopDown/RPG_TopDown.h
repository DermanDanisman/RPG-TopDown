// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/** Custom Depth Coloring */
// 250 - Enemy
// 249 - Player
// 251 - Items
#define CUSTOM_DEPTH_RED 250
#define CUSTOM_DEPTH_GREEN 249
#define CUSTOM_DEPTH_YELLOW 251

// Giving alias to the custom trace channel
#define ECC_Navigation ECC_GameTraceChannel1