/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNEAttributeCarrier.h
/// @author  Jakob Erdmann
/// @date    Mar 2011
/// @version $Id$
///
// Abstract Base class for gui objects which carry attributes
/****************************************************************************/
#ifndef GNEAttributeCarrier_h
#define GNEAttributeCarrier_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <fx.h>
#include <string>
#include <vector>
#include <map>
#include <utils/gui/settings/GUIVisualizationSettings.h>
#include <utils/xml/SUMOSAXHandler.h>
#include <utils/xml/SUMOXMLDefinitions.h>
#include <utils/common/UtilExceptions.h>
#include <utils/common/MsgHandler.h>
#include <utils/common/RGBColor.h>
#include <utils/common/SUMOVehicleClass.h>
#include <utils/common/ToString.h>
#include <utils/common/TplConvert.h>
#include <utils/gui/images/GUIIcons.h>

#include "GNEReferenceCounter.h"


// ===========================================================================
// class declarations
// ===========================================================================
class GNENet;
class GNEEdge;
class GNELane;
class GNEUndoList;
class GUIGlObject;

// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class GNEAttributeCarrier
 *
 * Abstract Base class for gui objects which carry attributes
 * inherits from GNEReferenceCounter for convenience
 */
class GNEAttributeCarrier : public GNEReferenceCounter {
    /// @brief declare friend class
    friend class GNEChange_Attribute;

public:
    enum TAGProperty {
        TAGPROPERTY_NETELEMENT = 1,
        TAGPROPERTY_ADDITIONAL = 2,
        TAGPROPERTY_SHAPE = 4,
        TAGPROPERTY_ROUTEELEMENT = 8,

        TAGPROPERTY_INTERNAL = 16,

        TAGPROPERTY_BLOCKMOVEMENT = 32,
        TAGPROPERTY_BLOCKSHAPE = 64,
        TAGPROPERTY_CLOSESHAPE = 128,
        TAGPROPERTY_GEOPOSITION = 256,
        TAGPROPERTY_GEOSHAPE = 512,
        TAGPROPERTY_DIALOG = 1024,
        TAGPROPERTY_PARENT = 2048,
    };

    /// @brief struct with the attribute Properties
    class TagValues {
    public:
        /// @brief default constructor
        TagValues();

        /// @brief parameter constructor
        TagValues(int tagProperty, SumoXMLTag tagParent = SUMO_TAG_NOTHING);

        bool isNetElement() const;

        bool isAdditional() const;

        bool isShape() const;

        bool canBlockMovement() const;

        bool canBlockShape() const;

        bool canCloseShape() const;

        bool hasGEOPosition() const;

        bool hasGEOShape() const;

        bool hasParent() const;

        bool hasDialog() const;

        SumoXMLTag getParentTag() const;

    private:
        /// @brief Property of attribute
        int myTagProperty;

        /// @brief parent tag
        SumoXMLTag myParentTag;
    };


    enum AttrProperty {
        ATTRPROPERTY_INT = 1,
        ATTRPROPERTY_FLOAT = 2,
        ATTRPROPERTY_BOOL = 4,
        ATTRPROPERTY_STRING = 8,
        ATTRPROPERTY_POSITION = 16,

        ATTRPROPERTY_COLOR = 64,
        ATTRPROPERTY_SVCPERMISSION = 128,
        ATTRPROPERTY_POSITIVE = 256,
        ATTRPROPERTY_UNIQUE = 512,
        ATTRPROPERTY_FILENAME = 1024,
        ATTRPROPERTY_NONEDITABLE = 2048,
        ATTRPROPERTY_DISCRETE = 4096,
        ATTRPROPERTY_PROBABILITY = 8192,
        ATTRPROPERTY_TIME = 16384,
        ATTRPROPERTY_ANGLE = 32768,
        ATTRPROPERTY_LIST = 65536,

        ATTRPROPERTY_OPTIONAL = 131072,
        ATTRPROPERTY_DEFAULTVALUE = 262144,
    };

    /// @brief struct with the attribute Properties
    class AttributeValues {
    public:
        /// @brief default constructor
        AttributeValues();

        /// @brief parameter constructor
        AttributeValues(int attributeProperty, const std::string &definition, const std::string &defaultValue, std::vector<std::string> discreteValues = std::vector<std::string>());

        /// @brief get restriction
        std::string getRestriction() const;

        /// brief get default value
        const std::string &getDefinition() const;

        /// brief get default value
        const std::string &getDefaultValue() const;

        std::string getType() const;

        bool isInt() const;

        bool isFloat() const;

        bool isBool() const;

        bool isString() const;

        bool isProbability() const;

        bool isNumerical() const;

        bool isTime() const;

        bool isPositive() const;

        bool isColor() const;

        bool isFilename() const;

        bool isSVC() const;

        bool isList() const;

        bool isUnique() const;

        bool isOptional() const;

        bool isDiscrete() const;

        const std::vector<std::string> &getDiscreteValues() const;

    private:
        /// @brief Property of attribute
        int myAttributeProperty;

        /// @brief text with a definition of attribute
        std::string myDefinition;

        /// @brief default value 
        std::string myDefaultValue;

        /// @brief discrete values
        std::vector<std::string> myDiscreteValues;
    };

    /**@brief Constructor
     * @param[in] tag SUMO Tag assigned to this type of object
     * @param[in] icon GUIIcon associated to the type of object
     */
    GNEAttributeCarrier(SumoXMLTag tag, GUIIcon icon);

    /// @brief Destructor
    virtual ~GNEAttributeCarrier() {};

    /// @brief This functions has to be implemented in all GNEAttributeCarriers
    /// @{
    /// @brief select attribute carrier using GUIGlobalSelection
    virtual void selectAttributeCarrier(bool changeFlag = true) = 0;

    /// @brief unselect attribute carrier using GUIGlobalSelection
    virtual void unselectAttributeCarrier(bool changeFlag = true) = 0;

    /// @brief check if attribute carrier is selected
    virtual bool isAttributeCarrierSelected() const = 0;

    /* @brief method for getting the Attribute of an XML key
     * @param[in] key The attribute key
     * @return string with the value associated to key
     */
    virtual std::string getAttribute(SumoXMLAttr key) const = 0;

    /* @brief method for setting the attribute and letting the object perform additional changes
     * @param[in] key The attribute key
     * @param[in] value The new value
     * @param[in] undoList The undoList on which to register changes
     * @param[in] net optionally the GNENet to inform about gui updates
     */
    virtual void setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) = 0;

    /* @brief method for setting the attribute and letting the object perform additional changes
    * @param[in] key The attribute key
    * @param[in] value The new value
    * @param[in] undoList The undoList on which to register changes
    */
    virtual bool isValid(SumoXMLAttr key, const std::string& value) = 0;
    /// @}

    /// @brief method for getting the attribute in the context of object selection
    virtual std::string getAttributeForSelection(SumoXMLAttr key) const;

    /// @brief get XML Tag assigned to this object
    SumoXMLTag getTag() const;

    /// @brief get FXIcon assigned to this object
    FXIcon* getIcon() const;

    /// @brief get GUI icon assigned to this object
    GUIIcon getGUIIcon() const;

    /// @brief get vector of attributes
    std::vector<SumoXMLAttr> getAttrs() const;

    /// @brief function to support debugging
    const std::string getID() const;

    /// @brief get all editable attributes for tag and their default values.
    static const std::map<SumoXMLAttr, GNEAttributeCarrier::AttributeValues>& allowedAttributes(SumoXMLTag tag);

    /// @brief get Tag Properties
    static const TagValues & getTagProperties(SumoXMLTag tag);

    /// @brief get all editable for tag elements of all types
    static std::vector<SumoXMLTag> allowedTags();

    /// @brief get all editable for tag net elements
    static const std::vector<SumoXMLTag>& allowedNetElementsTags();

    /// @brief get all editable for tag additional elements
    static const std::vector<SumoXMLTag>& allowedAdditionalTags();

    /// @brief get all editable for tag shape elements
    static const std::vector<SumoXMLTag>& allowedShapeTags();

    /// @brief check if an element with certain tag has a certain attribute
    static bool hasAttribute(SumoXMLTag tag, SumoXMLAttr attr);

    /// @brief check if attribute of an element has a default avlue
    static bool hasDefaultValue(SumoXMLTag tag, SumoXMLAttr attr);

    /// @brief return whether the given attribute allows for a combination of discrete values
    static bool discreteCombinableChoices(SumoXMLAttr attr);

    /// @brief return the number of attributes of the tag with the most highter number of attributes
    static int getHigherNumberOfAttributes();

    /// @brief return the default value of the attribute of an element
    /// @note It's advisable to check before with function hasDefaultValue if  exits a default value
    template<typename T>
    static T getDefaultValue(SumoXMLTag tag, SumoXMLAttr attr);

    /// @brief true if a number of type T can be parsed from string
    template<typename T>
    static bool canParse(const std::string& string) {
        try {
            parse<T>(string);
        } catch (NumberFormatException&) {
            return false;
        } catch (EmptyData&) {
            return false;
        } catch (BoolFormatException&) {
            return false;
        }
        return true;
    }

    /// @brief parses a number of type T from string
    template<typename T>
    static T parse(const std::string& string);

    /// @brief true if a positive number of type T can be parsed from string
    template<typename T>
    static bool isPositive(const std::string& string) {
        return canParse<T>(string) && parse<T>(string) > 0;
    }

    /// @brief parse a string of booleans (1 0 1 1....) using AND operation
    static bool parseStringToANDBool(const std::string& string);

    /// @brief true if value is a valid sumo ID
    static bool isValidID(const std::string& value);

    /// @brief true if value is a valid file value
    static bool isValidFilename(const std::string& value);

    /// @brief feature is still unchanged after being loaded (implies approval)
    static const std::string LOADED;

    /// @brief feature has been reguessed (may still be unchanged be we can't tell (yet)
    static const std::string GUESSED;

    /// @brief feature has been manually modified (implies approval)
    static const std::string MODIFIED;

    /// @brief feature has been approved but not changed (i.e. after being reguessed)
    static const std::string APPROVED;

    /// @brief default value for invalid positions (used by POIs and Polygons)
    static const double INVALID_POSITION;

    /// @brief Parse attribute from XML and show warnings if there are problems parsing it
    template <typename T>
    static T parseAttributeFromXML(const SUMOSAXAttributes& attrs, const std::string& objectID, const SumoXMLTag tag, const SumoXMLAttr attribute, bool& abort) {
        bool parsedOk = true;
        std::string defaultValue, parsedAttribute;
        // set additionalOfWarningMessage
        std::string additionalOfWarningMessage;
        if (objectID != "") {
            additionalOfWarningMessage = toString(tag) + " with ID '" + objectID + "'";
        } else {
            additionalOfWarningMessage = toString(tag);
        }
        // first check what kind of default value has to be give if parsing isn't valid (needed to avoid exceptions)
        if (allowedAttributes(tag).at(attribute).isNumerical()) {
            defaultValue = "0";
        } else if (allowedAttributes(tag).at(attribute).isColor()) {
            defaultValue = "BLACK";
        }
        // first check that attribute exists in XML
        if (attrs.hasAttribute(attribute)) {
            // First check if attribute can be parsed to string
            parsedAttribute = attrs.get<std::string>(attribute, objectID.c_str(), parsedOk, false);
            // check that sucesfully parsed attribute can be converted to type T
            if (parsedOk && !canParse<T>(parsedAttribute)) {
                parsedOk = false;
                // only set default value if this isn't a SVCPermission
                if(!allowedAttributes(tag).at(attribute).isSVC()) {
                    parsedAttribute = defaultValue;
                }
            }
            // declare a string for details about error formats
            std::string errorFormat;
            // set extra check for ID Values
            if (attribute == SUMO_ATTR_ID) {
                if (parsedAttribute.empty()) {
                    errorFormat = "ID cannot be empty; ";
                    parsedOk = false;
                } else if (isValidID(parsedAttribute) == false) {
                    errorFormat = "'" + parsedAttribute + "' contains invalid characters; ";
                    parsedOk = false;
                }
            }
            // Set extra checks for int values
            if (allowedAttributes(tag).at(attribute).isInt()) {
                if (canParse<int>(parsedAttribute)) {
                    // parse to int and check if can be negative
                    int parsedIntAttribute = parse<int>(parsedAttribute);
                    if (allowedAttributes(tag).at(attribute).isPositive() && parsedIntAttribute < 0) {
                        errorFormat = "Cannot be negative; ";
                        parsedOk = false;
                    }
                } else if (canParse<double>(parsedAttribute)) {
                    errorFormat = "Float cannot be reinterpreted as int; ";
                    parsedOk = false;
                } else {
                    errorFormat = "Cannot be parsed to int; ";
                    parsedOk = false;
                }
            }
            // Set extra checks for float(double) values
            if (allowedAttributes(tag).at(attribute).isFloat()) {
                if (canParse<double>(parsedAttribute)) {
                    // parse to double and check if can be negative
                    if (allowedAttributes(tag).at(attribute).isPositive() && parse<double>(parsedAttribute) < 0) {
                        errorFormat = "Cannot be negative; ";
                        parsedOk = false;
                    }
                } else {
                    errorFormat = "Cannot be parsed to float; ";
                    parsedOk = false;
                }
            }
            // set extra check for time(double) values
            if (allowedAttributes(tag).at(attribute).isTime()) {
                if (canParse<double>(parsedAttribute)) {
                    // parse to SUMO Real and check if is negative
                    if (parse<double>(parsedAttribute) < 0) {
                        errorFormat = "Time cannot be negative; ";
                        parsedOk = false;
                    }
                } else {
                    errorFormat = "Cannot be parsed to time; ";
                    parsedOk = false;
                }
            }
            // set extra check for probability values
            if (allowedAttributes(tag).at(attribute).isProbability()) {
                if (canParse<double>(parsedAttribute)) {
                    // parse to SUMO Real and check if is negative
                    if (parse<double>(parsedAttribute) < 0) {
                        errorFormat = "Probability cannot be smaller than 0; ";
                        parsedOk = false;
                    } else if (parse<double>(parsedAttribute) > 1) {
                        errorFormat = "Probability cannot be greather than 1; ";
                        parsedOk = false;
                    }
                } else {
                    errorFormat = "Cannot be parsed to probability; ";
                    parsedOk = false;
                }
            }
            // set extra check for color values
            if (allowedAttributes(tag).at(attribute).isColor() && !canParse<RGBColor>(parsedAttribute)) {
                errorFormat = "Invalid RGB format or named color; ";
                parsedOk = false;
            }
            // set extra check for filename values
            if (allowedAttributes(tag).at(attribute).isFilename() && (isValidFilename(parsedAttribute) == false)) {
                errorFormat = "Filename contains invalid characters; ";
                parsedOk = false;
            }
            // set extra check for SVCPermissions values
            if (allowedAttributes(tag).at(attribute).isSVC()) {
                if (canParseVehicleClasses(parsedAttribute)) {
                    parsedAttribute = toString(parseVehicleClasses(parsedAttribute));
                    parsedOk = true;
                } else {
                    errorFormat = "List of VClasses isn't valid; ";
                    parsedAttribute = defaultValue;
                    parsedOk = false;
                }
            }
            // set extra check for Vehicle Classes
            if ((!parsedOk) && (attribute == SUMO_ATTR_VCLASS)) {
                errorFormat = "Is not a part of defined set of Vehicle Classes; ";
            }
            // set extra check for Vehicle Classes
            if ((!parsedOk) && (attribute == SUMO_ATTR_GUISHAPE)) {
                errorFormat = "Is not a part of defined set of Gui Vehicle Shapes; ";
            }
            // set extra check for RouteProbes
            if ((attribute == SUMO_ATTR_ROUTEPROBE) && !isValidID(parsedAttribute)) {
                errorFormat = "RouteProbe ID contains invalid characters; ";
                parsedOk = false;
            }
            // If attribute has an invalid format
            if (!parsedOk) {
                // if attribute has a default value, obtain it as string. In other case, abort.
                if (getTagProperties(tag).canBlockMovement() && (attribute == GNE_ATTR_BLOCK_MOVEMENT)) {
                    // by default elements aren't blocked
                    parsedAttribute = "false";
                } else if (hasDefaultValue(tag, attribute)) {
                    parsedAttribute = toString(getDefaultValue<T>(tag, attribute));
                } else {
                    WRITE_WARNING("Format of essential " + allowedAttributes(tag).at(attribute).getType() + " attribute '" + toString(attribute) + "' of " +
                                  additionalOfWarningMessage +  " is invalid; " + errorFormat + toString(tag) + " cannot be created");
                    // abort parsing of element
                    abort = true;
                    // set default value
                    parsedAttribute = defaultValue;
                }
            }
        } else {
            // if attribute has a default value, obtain it. In other case, abort.
             if (getTagProperties(tag).canBlockMovement() && (attribute == GNE_ATTR_BLOCK_MOVEMENT)) {
                 // by default elements aren't blocked
                 parsedAttribute = "false";
             } else if (hasDefaultValue(tag, attribute)) {
                parsedAttribute = toString(getDefaultValue<T>(tag, attribute));
            } else {
                WRITE_WARNING("Essential " + allowedAttributes(tag).at(attribute).getType() + " attribute '" + toString(attribute) + "' of " +
                              additionalOfWarningMessage +  " is missing; " + toString(tag) + " cannot be created");
                // abort parsing of element
                abort = true;
                // set default value
                parsedAttribute = defaultValue;
            }
        }
        // return parsed attribute
        return parse<T>(parsedAttribute);
    }

    /// @name function used to parse GNEEdges and GNELanes
    /// @{

    /** @brief check if a list of edge IDs is valid
     * @brief value string with a list of edges
     * @brief report enable or disable show warning if edges aren't valid
     */
    static bool checkGNEEdgesValid(GNENet* net, const std::string& value, bool report);

    /**@brief check if a list of Lane IDs is valid
     * @brief value string with a list of lanes
     * @brief report enable or disable show warning if lanes aren't valid
     */
    static bool checkGNELanesValid(GNENet* net, const std::string& value, bool report);

    /**@brief parse string into vector of GNEEdges
    * @throw exception one of GNEEdges doesn't exist
    */
    static std::vector<GNEEdge*> parseGNEEdges(GNENet* net, const std::string& value);

    /**@brief parse string into vector of GNELanes
    * @throw exception one of GNELanes doesn't exist
    */
    static std::vector<GNELane*> parseGNELanes(GNENet* net, const std::string& value);

    /**@brief parse vector of GNEEdges into string
    * @throw exception one of GNEEdges doesn't exist
    */
    static std::string parseGNEEdges(const std::vector<GNEEdge*>& edges);

    /**@brief parse vector of GNELanes into string
    * @throw exception one of GNELanes doesn't exist
    */
    static std::string parseGNELanes(const std::vector<GNELane*>& lanes);

    /// @}

    /// @brief function to calculate circle resolution for all circles drawn in drawGL(...) functions
    static int getCircleResolution(const GUIVisualizationSettings& settings);

    /**@brief write attribute if is essential or if is optional AND is different of default value 
     * (Note: This solution is temporal, see #4049)
     */
    void writeAttribute(OutputDevice& device, SumoXMLAttr key) const;

protected:
    /// @brief boolean to check if this AC is selected (instead of GUIGlObjectStorage)
    bool mySelected;

private:

    /// @brief method for setting the attribute and nothing else (used in GNEChange_Attribute)
    virtual void setAttribute(SumoXMLAttr key, const std::string& value) = 0;

    /// @brief the xml tag to which this attribute carrier corresponds
    const SumoXMLTag myTag;

    /// @brief icon associated to this AC
    GUIIcon myIcon;

    /// @brief map with the allowed attributes and their default values
    static std::map<SumoXMLTag, std::pair<TagValues, std::map<SumoXMLAttr, AttributeValues> > > myAllowedAttributes;

    /// @brief vector with the allowed tags of netElements
    static std::vector<SumoXMLTag> myAllowedNetElementTags;

    /// @brief vector with the allowed tags of additionals
    static std::vector<SumoXMLTag> myAllowedAdditionalTags;

    /// @brief vector with the allowed tags of shapes
    static std::vector<SumoXMLTag> myAllowedShapeTags;

    /// @brief maximum number of attributes of all tags
    static int myMaxNumAttribute;

    /// @brief Invalidated assignment operator
    GNEAttributeCarrier& operator=(const GNEAttributeCarrier& src) = delete;
};

#endif

/****************************************************************************/

