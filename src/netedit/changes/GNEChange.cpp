/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2020 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    GNEChange.cpp
/// @author  Jakob Erdmann
/// @date    Mar 2011
///
// The reification of a NETEDIT editing operation (see command pattern)
// inherits from FXCommand and is used to for undo/redo
/****************************************************************************/

#include "GNEChange.h"

#include <netedit/elements/network/GNELane.h>
#include <netedit/elements/network/GNEEdge.h>
#include <netedit/elements/additional/GNEAdditional.h>
#include <netedit/elements/additional/GNEShape.h>
#include <netedit/elements/additional/GNETAZElement.h>
#include <netedit/elements/demand/GNEDemandElement.h>
#include <netedit/elements/data/GNEEdgeData.h>

// ===========================================================================
// FOX-declarations
// ===========================================================================

FXIMPLEMENT_ABSTRACT(GNEChange, FXCommand, nullptr, 0)


// ===========================================================================
// static
// ===========================================================================

const std::vector<GNEEdge*> GNEChange::myEmptyEdges = {};
const std::vector<GNELane*> GNEChange::myEmptyLanes = {};
const std::vector<GNEAdditional*> GNEChange::myEmptyAdditionals = {};
const std::vector<GNEShape*> GNEChange::myEmptyShapes = {};
const std::vector<GNETAZElement*> GNEChange::myEmptyTAZElements = {};
const std::vector<GNEDemandElement*> GNEChange::myEmptyDemandElements = {};
const std::vector<GNEGenericData*> GNEChange::myEmptyGenericDatas = {};


// ===========================================================================
// member method definitions
// ===========================================================================

GNEChange::GNEChange(bool forward) :
    myForward(forward),
    myParentEdges(myEmptyEdges),
    myParentLanes(myEmptyLanes),
    myParentAdditionals(myEmptyAdditionals),
    myParentShapes(myEmptyShapes),
    myParentTAZElements(myEmptyTAZElements),
    myParentDemandElements(myEmptyDemandElements),
    myParentGenericDatas(myEmptyGenericDatas),
    myChildEdges(myEmptyEdges),
    myChildLanes(myEmptyLanes),
    myChildAdditionals(myEmptyAdditionals),
    myChildShapes(myEmptyShapes),
    myChildTAZElements(myEmptyTAZElements),
    myChildDemandElements(myEmptyDemandElements),
    myChildGenericDatas(myEmptyGenericDatas) {}


GNEChange::GNEChange(GNEHierarchicalParentElements* parents, GNEHierarchicalChildElements* childs, bool forward) :
    myForward(forward),
    myParentEdges(parents->getParentEdges()),
    myParentLanes(parents->getParentLanes()),
    myParentAdditionals(parents->getParentAdditionals()),
    myParentShapes(parents->getParentShapes()),
    myParentTAZElements(parents->getParentTAZElements()),
    myParentDemandElements(parents->getParentDemandElements()),
    myParentGenericDatas(parents->getParentGenericDatas()),
    myChildEdges(childs->getChildEdges()),
    myChildLanes(childs->getChildLanes()),
    myChildAdditionals(childs->getChildAdditionals()),
    myChildShapes(childs->getChildShapes()),
    myChildTAZElements(childs->getChildTAZElements()),
    myChildDemandElements(childs->getChildDemandElements()),
    myChildGenericDatas(childs->getChildGenericDataElements()) {}


GNEChange::~GNEChange() {}


FXuint
GNEChange::size() const {
    return 1;
}


FXString
GNEChange::undoName() const {
    return "Undo";
}


FXString
GNEChange::redoName() const {
    return "Redo";
}


void
GNEChange::undo() {}


void
GNEChange::redo() {}


template <> void
GNEChange::addElementInParentsAndChildren(GNEEdge* edge) {
    // add edge in parent elements
    for (const auto& parent : myParentEdges) {
        parent->addChildElement(edge);
    }
    for (const auto& parent : myParentLanes) {
        parent->addChildElement(edge);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->addChildElement(edge);
    }
    for (const auto& parent : myParentShapes) {
        parent->addChildElement(edge);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->addChildElement(edge);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->addChildElement(edge);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->addChildElement(edge);
    }
    // add edge in child elements
    for (const auto& child : myChildEdges) {
        child->addParentEdge(edge);
    }
    for (const auto& child : myChildLanes) {
        child->addParentEdge(edge);
    }
    for (const auto& child : myChildAdditionals) {
        child->addParentEdge(edge);
    }
    for (const auto& child : myChildShapes) {
        child->addParentEdge(edge);
    }
    for (const auto& child : myChildTAZElements) {
        child->addParentEdge(edge);
    }
    for (const auto& child : myChildDemandElements) {
        child->addParentEdge(edge);
    }
    for (const auto& child : myChildGenericDatas) {
        child->addParentEdge(edge);
    }
}


template <> void
GNEChange::removeElementFromParentsAndChildren(GNEEdge* edge) {
    // Remove edge from parent elements
    for (const auto& parent : myParentEdges) {
        parent->removeChildElement(edge);
    }
    for (const auto& parent : myParentLanes) {
        parent->removeChildElement(edge);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->removeChildElement(edge);
    }
    for (const auto& parent : myParentShapes) {
        parent->removeChildElement(edge);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->removeChildElement(edge);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->removeChildElement(edge);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->removeChildElement(edge);
    }
    // Remove edge from child elements
    for (const auto& child : myChildEdges) {
        child->removeParentEdge(edge);
    }
    for (const auto& child : myChildLanes) {
        child->removeParentEdge(edge);
    }
    for (const auto& child : myChildAdditionals) {
        child->removeParentEdge(edge);
    }
    for (const auto& child : myChildShapes) {
        child->removeParentEdge(edge);
    }
    for (const auto& child : myChildTAZElements) {
        child->removeParentEdge(edge);
    }
    for (const auto& child : myChildDemandElements) {
        child->removeParentEdge(edge);
    }
    for (const auto& child : myChildGenericDatas) {
        child->removeParentEdge(edge);
    }
}


template <> void
GNEChange::addElementInParentsAndChildren(GNELane* lane) {
    // add lane in parent elements
    for (const auto& parent : myParentEdges) {
        parent->addChildElement(lane);
    }
    for (const auto& parent : myParentLanes) {
        parent->addChildElement(lane);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->addChildElement(lane);
    }
    for (const auto& parent : myParentShapes) {
        parent->addChildElement(lane);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->addChildElement(lane);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->addChildElement(lane);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->addChildElement(lane);
    }
    // add lane in child elements
    for (const auto& child : myChildEdges) {
        child->addParentLane(lane);
    }
    for (const auto& child : myChildLanes) {
        child->addParentLane(lane);
    }
    for (const auto& child : myChildAdditionals) {
        child->addParentLane(lane);
    }
    for (const auto& child : myChildShapes) {
        child->addParentLane(lane);
    }
    for (const auto& child : myChildTAZElements) {
        child->addParentLane(lane);
    }
    for (const auto& child : myChildDemandElements) {
        child->addParentLane(lane);
    }
    for (const auto& child : myChildGenericDatas) {
        child->addParentLane(lane);
    }
}


template <> void
GNEChange::removeElementFromParentsAndChildren(GNELane* lane) {
    // Remove lane from parent elements
    for (const auto& parent : myParentEdges) {
        parent->removeChildElement(lane);
    }
    for (const auto& parent : myParentLanes) {
        parent->removeChildElement(lane);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->removeChildElement(lane);
    }
    for (const auto& parent : myParentShapes) {
        parent->removeChildElement(lane);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->removeChildElement(lane);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->removeChildElement(lane);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->removeChildElement(lane);
    }
    // Remove lane from child elements
    for (const auto& child : myChildEdges) {
        child->removeParentLane(lane);
    }
    for (const auto& child : myChildLanes) {
        child->removeParentLane(lane);
    }
    for (const auto& child : myChildAdditionals) {
        child->removeParentLane(lane);
    }
    for (const auto& child : myChildShapes) {
        child->removeParentLane(lane);
    }
    for (const auto& child : myChildTAZElements) {
        child->removeParentLane(lane);
    }
    for (const auto& child : myChildDemandElements) {
        child->removeParentLane(lane);
    }
    for (const auto& child : myChildGenericDatas) {
        child->removeParentLane(lane);
    }
}


template <> void
GNEChange::addElementInParentsAndChildren(GNEShape* shape) {
    // add shape in parent elements
    for (const auto& parent : myParentEdges) {
        parent->addChildElement(shape);
    }
    for (const auto& parent : myParentLanes) {
        parent->addChildElement(shape);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->addChildElement(shape);
    }
    for (const auto& parent : myParentShapes) {
        parent->addChildElement(shape);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->addChildElement(shape);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->addChildElement(shape);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->addChildElement(shape);
    }
    // add shape in child elements
    for (const auto& child : myChildEdges) {
        child->addParentShape(shape);
    }
    for (const auto& child : myChildLanes) {
        child->addParentShape(shape);
    }
    for (const auto& child : myChildAdditionals) {
        child->addParentShape(shape);
    }
    for (const auto& child : myChildShapes) {
        child->addParentShape(shape);
    }
    for (const auto& child : myChildTAZElements) {
        child->addParentShape(shape);
    }
    for (const auto& child : myChildDemandElements) {
        child->addParentShape(shape);
    }
    for (const auto& child : myChildGenericDatas) {
        child->addParentShape(shape);
    }
}


template <> void
GNEChange::removeElementFromParentsAndChildren(GNEShape* shape) {
    // Remove shape from parent elements
    for (const auto& parent : myParentEdges) {
        parent->removeChildElement(shape);
    }
    for (const auto& parent : myParentLanes) {
        parent->removeChildElement(shape);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->removeChildElement(shape);
    }
    for (const auto& parent : myParentShapes) {
        parent->removeChildElement(shape);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->removeChildElement(shape);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->removeChildElement(shape);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->removeChildElement(shape);
    }
    // Remove shape from child elements
    for (const auto& child : myChildEdges) {
        child->removeParentShape(shape);
    }
    for (const auto& child : myChildLanes) {
        child->removeParentShape(shape);
    }
    for (const auto& child : myChildAdditionals) {
        child->removeParentShape(shape);
    }
    for (const auto& child : myChildShapes) {
        child->removeParentShape(shape);
    }
    for (const auto& child : myChildTAZElements) {
        child->removeParentShape(shape);
    }
    for (const auto& child : myChildDemandElements) {
        child->removeParentShape(shape);
    }
    for (const auto& child : myChildGenericDatas) {
        child->removeParentShape(shape);
    }
}


template <> void
GNEChange::addElementInParentsAndChildren(GNETAZElement* TAZElement) {
    // add shape in parent elements
    for (const auto& parent : myParentEdges) {
        parent->addChildElement(TAZElement);
    }
    for (const auto& parent : myParentLanes) {
        parent->addChildElement(TAZElement);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->addChildElement(TAZElement);
    }
    for (const auto& parent : myParentShapes) {
        parent->addChildElement(TAZElement);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->addChildElement(TAZElement);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->addChildElement(TAZElement);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->addChildElement(TAZElement);
    }
    // add shape in child elements
    for (const auto& child : myChildEdges) {
        child->addParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildLanes) {
        child->addParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildAdditionals) {
        child->addParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildShapes) {
        child->addParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildTAZElements) {
        child->addParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildDemandElements) {
        child->addParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildGenericDatas) {
        child->addParentTAZElement(TAZElement);
    }
}


template <> void
GNEChange::removeElementFromParentsAndChildren(GNETAZElement* TAZElement) {
    // Remove shape from parent elements
    for (const auto& parent : myParentEdges) {
        parent->removeChildElement(TAZElement);
    }
    for (const auto& parent : myParentLanes) {
        parent->removeChildElement(TAZElement);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->removeChildElement(TAZElement);
    }
    for (const auto& parent : myParentShapes) {
        parent->removeChildElement(TAZElement);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->removeChildElement(TAZElement);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->removeChildElement(TAZElement);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->removeChildElement(TAZElement);
    }
    // Remove shape from child elements
    for (const auto& child : myChildEdges) {
        child->removeParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildLanes) {
        child->removeParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildAdditionals) {
        child->removeParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildShapes) {
        child->removeParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildTAZElements) {
        child->removeParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildDemandElements) {
        child->removeParentTAZElement(TAZElement);
    }
    for (const auto& child : myChildGenericDatas) {
        child->removeParentTAZElement(TAZElement);
    }
}


template <> void
GNEChange::addElementInParentsAndChildren(GNEAdditional* additional) {
    // add additional in parent elements
    for (const auto& parent : myParentEdges) {
        parent->addChildElement(additional);
    }
    for (const auto& parent : myParentLanes) {
        parent->addChildElement(additional);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->addChildElement(additional);
    }
    for (const auto& parent : myParentShapes) {
        parent->addChildElement(additional);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->addChildElement(additional);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->addChildElement(additional);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->addChildElement(additional);
    }
    // add additional in child elements
    for (const auto& child : myChildEdges) {
        child->addParentAdditional(additional);
    }
    for (const auto& child : myChildLanes) {
        child->addParentAdditional(additional);
    }
    for (const auto& child : myChildAdditionals) {
        child->addParentAdditional(additional);
    }
    for (const auto& child : myChildShapes) {
        child->addParentAdditional(additional);
    }
    for (const auto& child : myChildTAZElements) {
        child->addParentAdditional(additional);
    }
    for (const auto& child : myChildDemandElements) {
        child->addParentAdditional(additional);
    }
    for (const auto& child : myChildGenericDatas) {
        child->addChildElement(additional);
    }
}


template <> void
GNEChange::removeElementFromParentsAndChildren(GNEAdditional* additional) {
    // Remove additional from parent elements
    for (const auto& parent : myParentEdges) {
        parent->removeChildElement(additional);
    }
    for (const auto& parent : myParentLanes) {
        parent->removeChildElement(additional);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->removeChildElement(additional);
    }
    for (const auto& parent : myParentShapes) {
        parent->removeChildElement(additional);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->removeChildElement(additional);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->removeChildElement(additional);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->removeChildElement(additional);
    }
    // Remove additional from child elements
    for (const auto& child : myChildEdges) {
        child->removeParentAdditional(additional);
    }
    for (const auto& child : myChildLanes) {
        child->removeParentAdditional(additional);
    }
    for (const auto& child : myChildAdditionals) {
        child->removeParentAdditional(additional);
    }
    for (const auto& child : myChildShapes) {
        child->removeParentAdditional(additional);
    }
    for (const auto& child : myChildTAZElements) {
        child->removeParentAdditional(additional);
    }
    for (const auto& child : myChildDemandElements) {
        child->removeParentAdditional(additional);
    }
    for (const auto& child : myChildGenericDatas) {
        child->removeParentAdditional(additional);
    }
}


template <> void
GNEChange::addElementInParentsAndChildren(GNEDemandElement* demandElement) {
    // add demandElement in parent elements
    for (const auto& parent : myParentEdges) {
        parent->addChildElement(demandElement);
    }
    for (const auto& parent : myParentLanes) {
        parent->addChildElement(demandElement);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->addChildElement(demandElement);
    }
    for (const auto& parent : myParentShapes) {
        parent->addChildElement(demandElement);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->addChildElement(demandElement);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->addChildElement(demandElement);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->addChildElement(demandElement);
    }
    // add demandElement in child elements
    for (const auto& child : myChildEdges) {
        child->addParentDemandElement(demandElement);
    }
    for (const auto& child : myChildLanes) {
        child->addParentDemandElement(demandElement);
    }
    for (const auto& child : myChildAdditionals) {
        child->addParentDemandElement(demandElement);
    }
    for (const auto& child : myChildShapes) {
        child->addParentDemandElement(demandElement);
    }
    for (const auto& child : myChildTAZElements) {
        child->addParentDemandElement(demandElement);
    }
    for (const auto& child : myChildDemandElements) {
        child->addParentDemandElement(demandElement);
    }
    for (const auto& child : myChildGenericDatas) {
        child->addParentDemandElement(demandElement);
    }
}


template <> void
GNEChange::removeElementFromParentsAndChildren(GNEDemandElement* demandElement) {
    // remove demandElement in parent elements
    for (const auto& parent : myParentEdges) {
        parent->removeChildElement(demandElement);
    }
    for (const auto& parent : myParentLanes) {
        parent->removeChildElement(demandElement);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->removeChildElement(demandElement);
    }
    for (const auto& parent : myParentShapes) {
        parent->removeChildElement(demandElement);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->removeChildElement(demandElement);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->removeChildElement(demandElement);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->removeChildElement(demandElement);
    }
    // remove demandElement in child elements
    for (const auto& child : myChildEdges) {
        child->removeParentDemandElement(demandElement);
    }
    for (const auto& child : myChildLanes) {
        child->removeParentDemandElement(demandElement);
    }
    for (const auto& child : myChildAdditionals) {
        child->removeParentDemandElement(demandElement);
    }
    for (const auto& child : myChildShapes) {
        child->removeParentDemandElement(demandElement);
    }
    for (const auto& child : myChildTAZElements) {
        child->removeParentDemandElement(demandElement);
    }
    for (const auto& child : myChildDemandElements) {
        child->removeParentDemandElement(demandElement);
    }
    for (const auto& child : myChildGenericDatas) {
        child->removeParentDemandElement(demandElement);
    }
}


template <> void
GNEChange::addElementInParentsAndChildren(GNEGenericData* genericData) {
    // add genericData in parent elements
    for (const auto& parent : myParentEdges) {
        parent->addChildElement(genericData);
    }
    for (const auto& parent : myParentLanes) {
        parent->addChildElement(genericData);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->addChildElement(genericData);
    }
    for (const auto& parent : myParentShapes) {
        parent->addChildElement(genericData);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->addChildElement(genericData);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->addChildElement(genericData);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->addChildElement(genericData);
    }
    // add genericData in child elements
    for (const auto& child : myChildEdges) {
        child->addParentGenericData(genericData);
    }
    for (const auto& child : myChildLanes) {
        child->addParentGenericData(genericData);
    }
    for (const auto& child : myChildAdditionals) {
        child->addParentGenericData(genericData);
    }
    for (const auto& child : myChildShapes) {
        child->addParentGenericData(genericData);
    }
    for (const auto& child : myChildTAZElements) {
        child->addParentGenericData(genericData);
    }
    for (const auto& child : myChildDemandElements) {
        child->addParentGenericData(genericData);
    }
    for (const auto& child : myChildGenericDatas) {
        child->addParentGenericData(genericData);
    }
}


template <> void
GNEChange::removeElementFromParentsAndChildren(GNEGenericData* genericData) {
    // remove genericData in parent elements
    for (const auto& parent : myParentEdges) {
        parent->removeChildElement(genericData);
    }
    for (const auto& parent : myParentLanes) {
        parent->removeChildElement(genericData);
    }
    for (const auto& parent : myParentAdditionals) {
        parent->removeChildElement(genericData);
    }
    for (const auto& parent : myParentShapes) {
        parent->removeChildElement(genericData);
    }
    for (const auto& parent : myParentTAZElements) {
        parent->removeChildElement(genericData);
    }
    for (const auto& parent : myParentDemandElements) {
        parent->removeChildElement(genericData);
    }
    for (const auto& parent : myParentGenericDatas) {
        parent->removeChildElement(genericData);
    }
    // remove genericData in child elements
    for (const auto& child : myChildEdges) {
        child->removeParentGenericData(genericData);
    }
    for (const auto& child : myChildLanes) {
        child->removeParentGenericData(genericData);
    }
    for (const auto& child : myChildAdditionals) {
        child->removeParentGenericData(genericData);
    }
    for (const auto& child : myChildShapes) {
        child->removeParentGenericData(genericData);
    }
    for (const auto& child : myChildTAZElements) {
        child->removeParentGenericData(genericData);
    }
    for (const auto& child : myChildDemandElements) {
        child->removeParentGenericData(genericData);
    }
    for (const auto& child : myChildGenericDatas) {
        child->removeParentGenericData(genericData);
    }
}


/****************************************************************************/
