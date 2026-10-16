/*
 * idea/sleela-native/sln_scene.h
 * Sleela-Native -- original real-time scene model (camera, moving light,
 * physical model, vignette, sound cues).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Clean-room, original code. No third-party source; depends only on our own
 * sln_math.h and the C++ standard library. This is the graphics-API-independent
 * MODEL of a real-time scene -- it is advanced on the CPU and is fully built
 * and verified without any GPU/driver. The renderer (sln_render_gl.*) consumes
 * this model and draws it with OpenGL; the audio engine (sln_audio.*) consumes
 * its sound cues.
 *
 * SPDX-License-Identifier: MIT  (see PROVENANCE.md)
 */
#ifndef SLN_SCENE_H
#define SLN_SCENE_H

#include "sln_math.h"

#include <cstdint>
#include <vector>

namespace sln {

/* ---- Camera ----------------------------------------------------------- */
/* An orbit camera: it looks at a target from a yaw/pitch/distance, which is
 * the natural control for inspecting a physical model in real time. */
struct Camera {
    Vec3  target{0, 0, 0};
    float distance = 6.0f;
    float yaw   = 0.6f;     /* radians, around Y */
    float pitch = 0.5f;     /* radians, above the horizon */
    float fovy  = 1.0472f;  /* 60 degrees */
    float znear = 0.05f;
    float zfar  = 100.0f;

    Vec3 eye() const {
        float cp = std::cos(pitch), sp = std::sin(pitch);
        float cy = std::cos(yaw),   sy = std::sin(yaw);
        return target + Vec3{ distance * cp * sy,
                              distance * sp,
                              distance * cp * cy };
    }
    Mat4 view() const { return look_at(eye(), target, Vec3{0, 1, 0}); }
    Mat4 projection(float aspect) const { return perspective(fovy, aspect, znear, zfar); }

    /* Smooth orbiting for the demo / auto-tour. */
    void orbit(float d_yaw, float d_pitch) {
        yaw += d_yaw; pitch += d_pitch;
        const float lim = 1.5533f; /* ~89 deg */
        if (pitch >  lim) pitch =  lim;
        if (pitch < -lim) pitch = -lim;
    }
};

/* ---- Moving light source --------------------------------------------- */
/* A point light that travels on an animated path (orbit + bob), with colour
 * and intensity. Its motion is what makes the shading feel alive. */
struct Light {
    Vec3  position{3, 4, 2};
    Vec3  color{1.0f, 0.95f, 0.85f};
    float intensity = 1.0f;
    float orbit_radius = 4.0f;
    float orbit_height = 3.0f;
    float orbit_speed  = 0.8f;   /* radians/second */
    float bob_speed    = 1.7f;
    float bob_amount   = 0.6f;
    float phase = 0.0f;          /* advanced by time */

    void advance(float dt) { phase += dt; }
    Vec3 world_position() const {
        return Vec3{ orbit_radius * std::cos(phase * orbit_speed),
                     orbit_height + bob_amount * std::sin(phase * bob_speed),
                     orbit_radius * std::sin(phase * orbit_speed) };
    }
};

/* ---- Vignette post-process ------------------------------------------- */
/* Darkens the frame toward the edges. Pure parameters the renderer's
 * fullscreen pass consumes. */
struct Vignette {
    bool  enabled   = true;
    float radius    = 0.75f;  /* where darkening starts, 0..1 of half-diagonal */
    float softness  = 0.45f;  /* feather width */
    float strength  = 0.6f;   /* 0 none .. 1 fully black at the corners */
};

/* ---- A physical model (procedural mesh) ------------------------------ */
/* A vertex of the model: position + normal (for lighting) + a base colour. */
struct Vertex {
    Vec3 pos;
    Vec3 normal;
    Vec3 color;
};

/* A simple rigid body: a mesh with a model transform that can spin in real
 * time. The default build procedure makes a lit, faceted icosphere-ish solid
 * plus a ground plane -- enough to show camera, lighting and vignette. */
struct Model {
    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;
    float spin = 0.0f;          /* current Y rotation (radians) */
    float spin_speed = 0.5f;    /* radians/second */

    Mat4 transform() const { return rotate_y(spin); }
    void advance(float dt) { spin += spin_speed * dt; }

    /* Append one triangle (flat-shaded: a shared face normal). */
    void add_triangle(const Vec3 &a, const Vec3 &b, const Vec3 &c, const Vec3 &col) {
        Vec3 n = normalize(cross(b - a, c - a));
        uint32_t base = (uint32_t)vertices.size();
        vertices.push_back({a, n, col});
        vertices.push_back({b, n, col});
        vertices.push_back({c, n, col});
        indices.push_back(base); indices.push_back(base + 1); indices.push_back(base + 2);
    }
};

/* ---- Sound cue ------------------------------------------------------- */
/* A scheduled sound: the scene emits cues (e.g. a soft tone each time the
 * light passes behind the model) that the audio engine renders. */
struct SoundCue {
    float freq_hz   = 220.0f;
    float gain      = 0.4f;
    float dur_s     = 0.25f;
    double at_time  = 0.0;   /* scene time to trigger */
    bool  fired     = false;
};

/* ---- The scene ------------------------------------------------------- */
struct Scene {
    Camera   camera;
    Light    light;
    Vignette vignette;
    Model    model;
    std::vector<SoundCue> cues;
    double   time = 0.0;      /* seconds since start */

    /* Build the default physical model: a faceted octahedron (the "solid")
     * and a ground quad. Original, procedural -- no imported asset. */
    void build_default() {
        model.vertices.clear();
        model.indices.clear();

        /* Octahedron solid, coloured by face. */
        const Vec3 top{0, 1.2f, 0}, bot{0, -1.2f, 0};
        const Vec3 n{0, 0, 1.2f}, s{0, 0, -1.2f}, e{1.2f, 0, 0}, w{-1.2f, 0, 0};
        const Vec3 c1{0.85f, 0.42f, 0.30f}, c2{0.30f, 0.55f, 0.85f};
        model.add_triangle(top, n, e, c1);
        model.add_triangle(top, e, s, c2);
        model.add_triangle(top, s, w, c1);
        model.add_triangle(top, w, n, c2);
        model.add_triangle(bot, e, n, c2);
        model.add_triangle(bot, s, e, c1);
        model.add_triangle(bot, w, s, c2);
        model.add_triangle(bot, n, w, c1);

        /* Ground plane (two triangles) a bit below the solid. */
        const float g = 6.0f, y = -1.3f;
        const Vec3 gc{0.14f, 0.15f, 0.18f};
        model.add_triangle({-g, y, -g}, { g, y, -g}, { g, y,  g}, gc);
        model.add_triangle({-g, y, -g}, { g, y,  g}, {-g, y,  g}, gc);

        /* A sound cue each half-orbit of the light. */
        cues.clear();
        for (int i = 1; i <= 8; ++i)
            cues.push_back(SoundCue{ 180.0f + 30.0f * i, 0.35f, 0.2f,
                                     (double)i * 1.9, false });
    }

    /* Advance everything by dt seconds (the real-time step). Returns the
     * number of sound cues that fired this step (for the audio engine). */
    int advance(float dt) {
        time += dt;
        model.advance(dt);
        light.advance(dt);
        camera.orbit(0.15f * dt, 0.0f);   /* gentle auto-tour */
        int fired = 0;
        for (auto &c : cues)
            if (!c.fired && time >= c.at_time) { c.fired = true; ++fired; }
        return fired;
    }
};

} // namespace sln

#endif /* SLN_SCENE_H */
