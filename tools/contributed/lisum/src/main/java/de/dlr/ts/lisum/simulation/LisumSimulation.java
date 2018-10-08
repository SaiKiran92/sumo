/****************************************************************************/
// Eclipse SUMO, LisumSimulation of Urban MObility; see https://eclipse.org/sumo
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

/*
 * Copyright (C) 2016
 * Deutsches Zentrum fuer Luft- und Raumfahrt e.V.
 * Institut fuer Verkehrssystemtechnik
 * 
 * German Aerospace Center
 * Institute of Transportation Systems
 * 
 */

import de.dlr.ts.lisum.interfaces.SimulationListener;
import de.dlr.ts.lisum.exceptions.LisumException;
import de.dlr.ts.lisum.interfaces.CityInterface;
import de.dlr.ts.lisum.lisa.Lisa;
import de.dlr.ts.lisum.sumo.Sumo;
import java.io.File;
import java.util.ArrayList;
import java.util.List;


/**
 *
 * @author @author <a href="mailto:maximiliano.bottazzi@dlr.de">Maximiliano Bottazzi</a>
 */
public class LisumSimulation implements SimulationListener
{
    public enum InitBeforePlayResponse {OK, LisaRESTfulServerNotFound}
    
    private Sumo sumo;
    private CityInterface cityInterface; //Lisa

    private SimulationDetectors detectors;    
    private SimulationControlUnits controlUnits;
        
    private SimulationFiles simulationFiles;

    private final List<SimulationStepListener> listeners = new ArrayList<>();
    private long currentSimulationStep = 0;
    
    private ConfigurationFile configurationFile = new ConfigurationFile();
    
    
    /**
     * 
     */
    public LisumSimulation()
    {
    }

    /**
     * 
     * @return 
     */
    public long getCurrentSimulationStep()
    {
        return currentSimulationStep;
    }

    /**
     * 
     * @return 
     */
    public ConfigurationFile getConfigurationFile()
    {
        return configurationFile;
    }
    
    /**
     * 
     * @param lisumConfigurationFile
     * @throws LisumException 
     */
    public void load(File lisumConfigurationFile) throws LisumException, Exception
    {
        configurationFile = new ConfigurationFile();
        configurationFile.read(lisumConfigurationFile);
        
        simulationFiles = new SimulationFiles();
        simulationFiles.read(lisumConfigurationFile);
        simulationFiles.setLisaDataDirectory(configurationFile.getLisaDirectory());
        
        load();
    }

    /**
     * 
     * @param listener 
     */
    public void addListener(SimulationStepListener listener)
    {
        listeners.add(listener);
    }
    
    /**
     * 
     * @return 
     */
    public CityInterface getCityInterface()
    {
        return cityInterface;
    }
    
    /**
     * 
     * @return 
     */
    public SimulationDetectors getDetectors()
    {
        return detectors;
    }

    /**
     * 
     * @return 
     */
    public Sumo getSumo()
    {
        return sumo;
    }
    
    
    /**
     * 
     * @throws java.lang.Exception
     */
    public void load() throws Exception
    {
        /**
         * 
         */
        sumo = new Sumo(this);
        sumo.initBeforePlay();
        
        /**
         * 
         */
        cityInterface = Lisa.create();
        cityInterface.load(simulationFiles.getLisaDataDirectory());        
        
        controlUnits = new SimulationControlUnits();
        controlUnits.load(configurationFile, cityInterface);
        
        detectors = new SimulationDetectors();
        detectors.load(configurationFile, cityInterface);
    }

    /**
     * 
     * @return 
     */
    public InitBeforePlayResponse initBeforePlay()
    {
        InitBeforePlayResponse initBeforePlay = cityInterface.initBeforePlay();
        
        if(initBeforePlay != InitBeforePlayResponse.OK)
            return initBeforePlay;
        
        currentSimulationStep = 0;
        sumo.initBeforePlay();
        
        return InitBeforePlayResponse.OK;
    }
    
    /**
     * 
     * @return 
     */
    public Runnable getRunnable()
    {
        return sumo.getRunnable();
    }
    
    /**
     * 
     * @return 
     */
    public SimulationFiles getSimulationFiles()
    {
        return simulationFiles;
    }

    /**
     * 
     * @return 
     */
    public SimulationControlUnits getControlUnits()
    {
        return controlUnits;
    }
    
    
    /**
     * Here happens all the magic
     * 
     */
    @Override
    public void executeSimulationStep(long simulationStep)
    {
        this.currentSimulationStep = simulationStep;
        
        cityInterface.executeSimulationStep(simulationStep);
        
        for (SimulationStepListener listener : listeners)
            listener.step(simulationStep);
    }
 
    /**
     * 
     */
    public interface SimulationStepListener
    {
        void step(long simulationStep);
    }

}
