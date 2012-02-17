#ifndef PPH_PREDATOR_H
#define PPH_PREDATOR_H

#include "Agent.h"
#include "network.h"

namespace PredatorPreyHunter {
  class Predator : public Agent {
    NEAT::Network* ptrNetwork;
    public:
      Predator( const GridWorld* ptrGridWorld, const uint& agentId, const Position& p, NEAT::Network* ptrNetwork ); 
      virtual Position move( const std::vector<AgentInformation>& vAgentInformation );
  };
}

#endif // PPH_PREDATOR_H
