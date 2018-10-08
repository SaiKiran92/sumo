/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2016-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    Constants.java
/// @author  Maximiliano Bottazzi
/// @date    2016
/// @version $Id$
///
//
/****************************************************************************/
package de.dlr.ts.lisum.simulation;

import de.dlr.ts.utils.xmladmin2.XMLAdmin2;
import de.dlr.ts.utils.xmladmin2.XMLNode;
import de.dlr.ts.utils.xmladmin2.exceptions.MalformedKeyOrNameException;
import de.dlr.ts.utils.xmladmin2.exceptions.XMLNodeNotFoundException;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import org.xml.sax.SAXException;

/**
 *
 * @author @author <a href="mailto:maximiliano.bottazzi@dlr.de">Maximiliano Bottazzi</a>
 */
public class VehicleTypes
{
    private final Map<String, VehicleType> types = new HashMap<>();
    
    
    /**
     * 
     */
    public VehicleTypes()
    {
    }
    
    /**
     * 
     * @param name
     * @return 
     */
    public VehicleType getVehicleType(String name)
    {
        return types.get(name);
    }
    
    /**
     * 
     * @param configFile 
     */
    public void load(File configFile)
    {
        try
        {
            XMLAdmin2 x = new XMLAdmin2().load(configFile);
            
            int nodesCount = x.getNodesCount("vType");
            
            for (int i = 0; i < nodesCount; i++)
            {
                XMLNode node = x.getNode("vType", i);
                String id = node.getAttributes().get("id").getValue();
                int length = node.getAttributes().get("length").getValue(0);
                
                types.put(id, new VehicleType(id, length));
            }
        }
        catch (SAXException | IOException | MalformedKeyOrNameException | XMLNodeNotFoundException ex)
        {
            ex.printStackTrace(System.out);
        }
    }
    
    /**
     * 
     */
    public static class VehicleType
    {
        private final String name;
        private final int length;

        /**
         * 
         * @param name
         * @param length 
         */
        public VehicleType(String name, int length)
        {
            this.name = name;
            this.length = length;
        }

        public String getName() {
            return name;
        }
        
        /**
         * 
         * @return 
         */
        public int getLength()
        {
            return length;
        }
        
        
    }
}
