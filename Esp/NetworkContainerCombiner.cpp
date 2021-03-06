/*
 * NetworkContainerSubtask.cpp
 *
 *  Created on: Feb 17, 2012
 *      Author: anand
 */

#include "NetworkContainerCombiner.h"
#include "FeedForwardNetwork.h"

namespace EspPredPreyHunter
{
    NetworkContainerCombiner::NetworkContainerCombiner()
            : NetworkContainer(0, 0, 0), networkContainers(0)
    {
        networkContainerType = COMBINER;
    }

    NetworkContainerCombiner::NetworkContainerCombiner(const uint& nHiddenNeurons,
            const uint& popSize, const uint& netTp, const uint& numNetworks,
            const uint& numInputsPerNetwork, const uint& numOutputsPerNetwork)
            : NetworkContainer(numInputsPerNetwork, numOutputsPerNetwork,
                    numNetworks + (numNetworks == 1 ? 0 : 1))
    {
        LOG(INFO) << "Creating combiner network with num inputs " << numInputsPerNetwork
                << " and num outputs " << numOutputsPerNetwork;
        combinerNetwork = new FeedForwardNetwork(nHiddenNeurons, popSize, numInputsPerNetwork,
                numOutputsPerNetwork, true);
        networkContainerType = COMBINER;
    }

    NetworkContainerCombiner::~NetworkContainerCombiner()
    {
    }

    void NetworkContainerCombiner::initializeNetworks()
    {
        combinerNetwork->setNeurons();
    }

    void NetworkContainerCombiner::setFitness(const double& fitness)
    {
        VLOG(5) << "Setting fitness";
        combinerNetwork->setFitness(fitness);
    }

    void NetworkContainerCombiner::incrementTests()
    {
        combinerNetwork->incrementTests();
    }

    void NetworkContainerCombiner::average()
    {
        VLOG(5) << "Averaging.";
        combinerNetwork->average();
    }

    void NetworkContainerCombiner::qsortNeurons()
    {
        VLOG(5) << "Sorting.";
        combinerNetwork->qsortNeurons();
    }

    void NetworkContainerCombiner::mutate()
    {
        VLOG(5) << "Doing mutate";
        combinerNetwork->mutate();
    }

    void NetworkContainerCombiner::evalReset()
    {
        combinerNetwork->evalReset();
    }

    void NetworkContainerCombiner::recombineHallOfFame(NetworkContainer* hallOfFameNetwork)
    {
        VLOG(5) << "Doing recombine hall of fame";
        vector<Network*> hallOfFameNetworks = hallOfFameNetwork->getNetworks();
        if (hallOfFameNetworks.size() != 1) {
            LOG(FATAL) << "There can't be more than one hall of fame network!";
        }
        combinerNetwork->recombineHallOfFame(hallOfFameNetworks[0]);
    }

    vector<Network*> NetworkContainerCombiner::getNetworks() const
    {
        vector<Network*> tempNetworks = vector<Network*>();
        tempNetworks.push_back(combinerNetwork);
        return tempNetworks;
    }

    vector<NetworkContainer*> NetworkContainerCombiner::getNetworkContainers() const
    {
        return networkContainers;
    }

    void NetworkContainerCombiner::setNetworkContainers(
            const vector<NetworkContainer*>& networkContainers)
    {
        this->networkContainers = networkContainers;
    }

    void NetworkContainerCombiner::activate(const vector<double>& input, vector<double>& output)
    {
        if (networkContainers.size() == 0)
            LOG(FATAL) << "Trying to activate empty container!";

        const uint indepInputs = totalNumInputs
                - (networkContainers[0]->getOutputsPerNetwork() * networkContainers.size());
        vector<double> newInput(indepInputs);

        newInput.assign(input.begin(), input.begin() + indepInputs);

        vector<double> tempSingleOutputs(newInput);

        const int k = networkContainers[0]->getInputsPerNetwork();
        const int l = input.size() / networkContainers.size();
        const int m = networkContainers[0]->getOutputsPerNetwork();

        if (k + 1 != l) {
            LOG(ERROR) << "Input size doesn't match!";
        }

        for (uint i = 0; i < networkContainers.size(); i++) {
            vector<double> tempOutput = vector<double>(m);
            vector<double> tempInput = vector<double>();
            for (int j = 0; j < k; j++) {
                tempInput.push_back(input[l * i + j]);
            }
            networkContainers[i]->activate(tempInput, tempOutput);
            tempSingleOutputs.insert(tempSingleOutputs.end(), tempOutput.begin(), tempOutput.end());
        }

        // Finally there is always one network
        combinerNetwork->activate(tempSingleOutputs, output);
    }
}

