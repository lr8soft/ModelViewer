#pragma once

// EventData == model path (const char*)
#define EVENT_LOAD_NEW_MODEL "R_EVENT_LOAD_NEW_MODEL"
// EventData == shader path (const char*)
#define EVENT_LOAD_SHADER "R_EVENT_LOAD_SHADER"
// EventData == RenderData
#define EVENT_RENDER_MODEL "R_EVENT_RENDER_MODEL"
// EventData == model path (const char*)
#define EVENT_STOP_RENDER_MODEL "R_EVENT_STOP_RENDER_MODEL"

// EventData == UniformData*
#define EVENT_SEND_UNIFORM_DATA "R_EVENT_SEND_UNIFORM_DATA"
// EventData == shader path (const char*)
#define EVENT_SEND_UNIFORM_CAMERA_DATA "R_EVENT_SEND_UNIFORM_CAMERA_DATA"