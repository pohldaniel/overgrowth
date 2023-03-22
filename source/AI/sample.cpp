//-----------------------------------------------------------------------------
//           Name: sample.cpp
//      Developer: External
//         Author:
//    Description: This is a utility file from the Recast project which has been
//                 extracted and modified by Wolfire Games LLC
//        License: Read below
//-----------------------------------------------------------------------------

//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include "sample.h"
#include "input_geom.h"
#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "DetourCrowd.h"

#ifdef WIN32
#define snprintf _snprintf
#endif

Sample::Sample() : m_geom(0),
                   m_navMesh(0),
                   m_navQuery(0),
                   m_crowd(0),
                   m_tool(0),
                   m_ctx(0) {
    resetCommonSettings();
    m_navQuery = dtAllocNavMeshQuery();
    m_crowd = dtAllocCrowd();

    for (auto& toolState : m_toolStates)
        toolState = 0;
}

Sample::~Sample() {
    dtFreeNavMeshQuery(m_navQuery);
    dtFreeNavMesh(m_navMesh);
    dtFreeCrowd(m_crowd);
    delete m_tool;
    for (auto& toolState : m_toolStates)
        delete toolState;
}

void Sample::setTool(SampleTool* tool) {
    delete m_tool;
    m_tool = tool;
    if (tool)
        m_tool->init(this);
}

void Sample::handleSettings() {
}

void Sample::handleTools() {
}

void Sample::handleDebugMode() {
}

void Sample::handleRender() {
}

void Sample::handleRenderOverlay(double* /*proj*/, double* /*model*/, int* /*view*/) {
}

void Sample::handleMeshChanged(InputGeom* geom) {
    m_geom = geom;

    const BuildSettings* buildSettings = geom->getBuildSettings();
    if (buildSettings) {
        m_cellSize = buildSettings->cellSize;
        m_cellHeight = buildSettings->cellHeight;
        m_agentHeight = buildSettings->agentHeight;
        m_agentRadius = buildSettings->agentRadius;
        m_agentMaxClimb = buildSettings->agentMaxClimb;
        m_agentMaxSlope = buildSettings->agentMaxSlope;
        m_regionMinSize = buildSettings->regionMinSize;
        m_regionMergeSize = buildSettings->regionMergeSize;
        m_edgeMaxLen = buildSettings->edgeMaxLen;
        m_edgeMaxError = buildSettings->edgeMaxError;
        m_vertsPerPoly = buildSettings->vertsPerPoly;
        m_detailSampleDist = buildSettings->detailSampleDist;
        m_detailSampleMaxError = buildSettings->detailSampleMaxError;
        m_partitionType = buildSettings->partitionType;
    }
}

void Sample::collectSettings(BuildSettings& settings) {
    settings.cellSize = m_cellSize;
    settings.cellHeight = m_cellHeight;
    settings.agentHeight = m_agentHeight;
    settings.agentRadius = m_agentRadius;
    settings.agentMaxClimb = m_agentMaxClimb;
    settings.agentMaxSlope = m_agentMaxSlope;
    settings.regionMinSize = m_regionMinSize;
    settings.regionMergeSize = m_regionMergeSize;
    settings.edgeMaxLen = m_edgeMaxLen;
    settings.edgeMaxError = m_edgeMaxError;
    settings.vertsPerPoly = m_vertsPerPoly;
    settings.detailSampleDist = m_detailSampleDist;
    settings.detailSampleMaxError = m_detailSampleMaxError;
    settings.partitionType = m_partitionType;
}

void Sample::resetCommonSettings() {
    m_cellSize = 0.3f;
    m_cellHeight = 0.2f;
    m_agentHeight = 2.0f;
    m_agentRadius = 0.6f;
    m_agentMaxClimb = 0.9f;
    m_agentMaxSlope = 45.0f;
    m_regionMinSize = 8;
    m_regionMergeSize = 20;
    m_edgeMaxLen = 12.0f;
    m_edgeMaxError = 1.3f;
    m_vertsPerPoly = 6.0f;
    m_detailSampleDist = 6.0f;
    m_detailSampleMaxError = 1.0f;
    m_partitionType = SAMPLE_PARTITION_WATERSHED;
}

void Sample::handleCommonSettings() {
    if (m_geom) {
        const float* bmin = m_geom->getNavMeshBoundsMin();
        const float* bmax = m_geom->getNavMeshBoundsMax();
        int gw = 0, gh = 0;
        rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
        char text[64];
        snprintf(text, 64, "Voxels  %d x %d", gw, gh);
    }
}

void Sample::handleClick(const float* s, const float* p, bool shift) {
    if (m_tool)
        m_tool->handleClick(s, p, shift);
}

void Sample::handleToggle() {
    if (m_tool)
        m_tool->handleToggle();
}

void Sample::handleStep() {
    if (m_tool)
        m_tool->handleStep();
}

bool Sample::handleBuild() {
    return true;
}

void Sample::handleUpdate(const float dt) {
    if (m_tool)
        m_tool->handleUpdate(dt);
    updateToolStates(dt);
}

void Sample::updateToolStates(const float dt) {
    for (auto& toolState : m_toolStates) {
        if (toolState)
            toolState->handleUpdate(dt);
    }
}

void Sample::initToolStates(Sample* sample) {
    for (auto& toolState : m_toolStates) {
        if (toolState)
            toolState->init(sample);
    }
}

void Sample::resetToolStates() {
    for (auto& toolState : m_toolStates) {
        if (toolState)
            toolState->reset();
    }
}

void Sample::renderToolStates() {
    for (auto& toolState : m_toolStates) {
        if (toolState)
            toolState->handleRender();
    }
}

void Sample::renderOverlayToolStates(double* proj, double* model, int* view) {
    for (auto& toolState : m_toolStates) {
        if (toolState)
            toolState->handleRenderOverlay(proj, model, view);
    }
}
