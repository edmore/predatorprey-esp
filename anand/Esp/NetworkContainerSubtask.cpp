/*
 * NetworkContainerSubtask.cpp
 *
 *  Created on: Feb 17, 2012
 *      Author: anand
 */

#include "NetworkContainerSubtask.h"
#include "FeedForwardNetwork.h"

namespace EspPredPreyHunter
{
    NetworkContainerSubtask::NetworkContainerSubtask()
            : NetworkContainer(0, 0, 0), networkContainers(0)
    {
    }

    NetworkContainerSubtask::NetworkContainerSubtask(const uint& nHiddenNeurons,
            const uint& popSize, const uint& netTp, const uint& numNetworks,
            const uint& numInputsPerNetwork, const uint& numOutputsPerNetwork)
            : NetworkContainer(numInputsPerNetwork,
                    numOutputsPerNetwork, numNetworks + (numNetworks == 1 ? 0 : 1))
    {
        combinerNetwork = new FeedForwardNetwork(nHiddenNeurons, popSize, numInputsPerNetwork,
                numOutputsPerNetwork);
    }

    NetworkContainerSubtask::~NetworkContainerSubtask()
    {
    }

    void NetworkContainerSubtask::initializeNetworks()
    {
        combinerNetwork->setNeurons();
    }

    void NetworkContainerSubtask::setFitness(const double& fitness)
    {
        VLOG(5) << "Setting fitness";
        combinerNetwork->setFitness(fitness);
    }

    void NetworkContainerSubtask::incrementTests()
    {
        combinerNetwork->incrementTests();
    }

    void NetworkContainerSubtask::average()
    {
        VLOG(5) << "Averaging.";
        combinerNetwork->average();
    }

    void NetworkContainerSubtask::qsortNeurons()
    {
        VLOG(5) << "Sorting.";
        combinerNetwork->qsortNeurons();
    }

    void NetworkContainerSubtask::mutate()
    {
        VLOG(5) << "Doing mutate";
        combinerNetwork->mutate();
    }

    void NetworkContainerSubtask::evalReset()
    {
        combinerNetwork->evalReset();
    }

    void NetworkContainerSubtask::recombineHallOfFame(NetworkContainer* hallOfFameNetwork)
    {
        VLOG(5) << "Doing recombine hall of fame";
        vector<Network*> hallOfFameNetworks = hallOfFameNetwork->getNetworks();
        if (hallOfFameNetworks.size() != 1) {
            LOG(FATAL) << "There can't be more than one hall of fame network!";
        }
        combinerNetwork->recombineHallOfFame(hallOfFameNetworks[0]);
    }

    vector<Network*> NetworkContainerSubtask::getNetworks() const
    {
        vector<Network*> tempNetworks = vector<Network*>();
        tempNetworks.push_back(combinerNetwork);
        return tempNetworks;
    }

    vector<NetworkContainer*> NetworkContainerSubtask::getNetworkContainers() const
    {
        return networkContainers;
    }

    /*
    void NetworkContainerSubtask::setNetwork(const NetworkContainer& networkContainer)
    {
        // NOTE: This will be just one network!
        vector<Network*> networks = networkContainer.getNetworks();
        if (networks.size() != 1) {
            LOG(FATAL) << "There can't be more than one hall of fame network!";
        }
        combinerNetwork = networks[0];
    }*/

    void NetworkContainerSubtask::setNetworkContainers(const vector<NetworkContainer*>& networkContainers)
    {
        this->networkContainers = networkContainers;
    }

    void NetworkContainerSubtask::activate(const vector<double>& input, vector<double>& output)
    {
        if (networkContainers.size() == 0)
            LOG(FATAL) << "Trying to activate empty container!";

        vector<double> tempSingleOutputs(input);
        int k = input.size() / networkContainers.size();
        VLOG(4) << "k is " << k;
        for (uint i = 0; i < networkContainers.size(); i++) {
            vector<double> tempOutput = vector<double>(5);
            vector<double> tempInput = vector<double>();
            for (int j = 0; j < k; j++) {
                tempInput.push_back(input[k * i + j]);
            }
            networkContainers[i]->activate(tempInput, tempOutput);
            tempSingleOutputs.insert(tempSingleOutputs.end(), tempOutput.begin(), tempOutput.end());
        }

        // Finally there is always one network
        combinerNetwork->activate(tempSingleOutputs, output);
    }
}

