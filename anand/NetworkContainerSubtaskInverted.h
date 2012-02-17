/*
 * NetworkContainerSubtaskInverted.h
 *
 *  Created on: Feb 17, 2012
 *      Author: anand
 */

#ifndef NETWORKCONTAINERSUBTASKINVERTED_H_
#define NETWORKCONTAINERSUBTASKINVERTED_H_

#include "Network.h"
#include "NetworkContainer.h"

namespace EspPredPreyHunter
{
    using std::vector;

    /**
     * This is the network combiner for the multi-agent ESP case. This is always used, irrespective
     * of the combination mechanism of the sub-tasks
     */
    class NetworkContainerSubtaskInverted: public NetworkContainer
    {
        vector<NetworkContainer*> networkContainers;
        vector<Network*> networks;
    public:
        NetworkContainerSubtaskInverted();
        NetworkContainerSubtaskInverted(const vector<NetworkContainer*> networkContainers);
        ~NetworkContainerSubtaskInverted();

        void setNetwork(const NetworkContainer& networkContainer, const bool& append);

        void setFitness(const double& fitness);

        void incrementTests();

        void average();

        // For subpop
        void qsortNeurons();

        // For subpop
        void mutate();

        void recombineHallOfFame(NetworkContainer* hallOfFameNetwork);

        vector<Network*> getNetworks() const;

        /**
         * Returns the output from the given input and outputs
         * @param input
         * @param output
         * @return
         */
        void activate(const vector<double>& input, vector<double>& output);
    };
}


#endif /* NETWORKCONTAINERSUBTASKINVERTED_H_ */