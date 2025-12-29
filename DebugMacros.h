#pragma once
//#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(location) DrawDebugSphere(GetWorld(), location, 20.f, 15, FColor::Red, true, 5.f, 0, 0.5f)
#define DRAW_LINE(StartLocation, EndLocation) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, 5.f, 0, 10.f)
#define DRAW_POINT(location) DrawDebugPoint(GetWorld(), location, 50.f, FColor::Green, true, 5.f)
#define DRAW_VECTOR(StartLocation, EndLocation) \
    { \
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, 5.f, 0, 10.f);  \
        DrawDebugPoint(GetWorld(), EndLocation, 50.f, FColor::Yellow, true, 5.f);  \
    }

#define DRAW_SPHERE_SingleFrame(location) DrawDebugSphere(GetWorld(), location, 20.f, 15, FColor::Red, false, 0, 0, 3.f)
#define DRAW_LINE_SingleFrame(StartLocation, EndLocation) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0, 0, 10.f)
#define DRAW_POINT_SingleFrame(location) DrawDebugPoint(GetWorld(), location, 50.f, FColor::Green, false, 0)
#define DRAW_VECTOR_SingleFrame(StartLocation, EndLocation) \
    { \
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0, 0, 10.f);  \
        DrawDebugPoint(GetWorld(), EndLocation, 50.f, FColor::Yellow, false, 0);  \
    }