/*
 * Environment.h
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#ifndef PATTERN_ENVIRONMENT_H_
#define PATTERN_ENVIRONMENT_H_

#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include "Signature.h"
#include "mixture/Mixture.h"
#include "Dependencies.h"
#include "../simulation/Rule.h"
#include "../spatial/Channel.h"
#include "../spatial/Compartment.h"
//#include "../state/AlgExpression.h"
//#include "../grammar/ast/Basics.h"
#include "../util/Exceptions.h"

namespace grammar {
namespace ast {
class Id;
}
}

namespace expressions {
template <typename T> class Auxiliar;
}

namespace simulation {
class Perturbation;
}

namespace pattern {

using namespace std;

using Id = grammar::ast::Id;

typedef unsigned short short_id;

/** \brief All the mappings for names and ids needed by the simulation.
 *
 * Manipulate and store all the mappings between names and ids of
 * elements declared with kappa. Global elements are stored first
 * to broadcast the global environment among MPI processes.
 */
class Environment {
protected:
	typedef unordered_map<string,short> IdMap;
	//static Environment env;
	//IdMap algMap, kappaMap;
	IdMap varMap, compartmentMap, channelMap, signatureMap;
	unordered_map<string,unsigned> tokenMap;
	//vector<string> algNames, kappaNames;
	vector<string> varNames, tokenNames, compartmentNames;
	vector<Signature> signatures;
	vector<Compartment> compartments;
	vector<list<Channel> > channels;
	vector<UseExpression> useExpressions;
	list<Mixture*> mixtures;
	list<Mixture::Component*> components;
	vector<list<Mixture::Agent*> > agentPatterns;
	vector<simulation::Rule> rules;
	vector<simulation::Perturbation*> perts;
	list<state::Variable*> observables;
	map<string,tuple<Mixture*,small_id,small_id>> auxTemp;
	map<const Mixture*,list<expressions::Auxiliar<FL_TYPE>*>> auxExpressions;
	mutable Dependencies deps;//mutable because [] accessing

	//(ag_id,site) -> list { (cc,ag_id) } map to every freeSite for side-effect events?
	unordered_map<int,list<pair<const Pattern::Component*,small_id> > > freeSiteCC;

	bool exists(const string &name,const IdMap &map);
public:
	//inline static Environment& getInstance() {return env;}
	Environment();
	~Environment();

	unsigned declareToken(const grammar::ast::Id &name);
	short declareVariable(const grammar::ast::Id &name,bool isKappa);
	Signature& declareSignature(const grammar::ast::Id& sign);
	Compartment& declareCompartment(const grammar::ast::Id& comp);
	UseExpression& declareUseExpression(unsigned short id,size_t n);
	Channel& declareChannel(const grammar::ast::Id &channel);
	void declareMixture(Mixture* &m);
	map<small_id,small_id> declareComponent(Pattern::Component* &c);

	/** \brief Declare an Agent as a kappa-pattern
	 *
	 * The agent-patterns in the environment are unique. If an equal
	 * pattern is found, new_agent will be deleted and the reference
	 * will be changed to point to the existing agent.
	 * @param new_agent a reference to the new Agent pointer.		 */
	void declareAgentPattern(Mixture::Agent* &new_agent);
	simulation::Rule& declareRule(const grammar::ast::Id &name,Mixture& mix,
			const yy::location& loc);
	void declarePert(simulation::Perturbation* pert);

	void declareObservable(state::Variable* obs);

	void buildInfluenceMap(const VarVector& vars);
	void buildFreeSiteCC();
	const list<pair<const Mixture::Component*,small_id> >& getFreeSiteCC(small_id ag,small_id site) const;

	//template <typename T>
	//const T& getSafe(grammar::ast::Id& name) const;

	const Signature& getSignature(short id) const;
	const vector<Signature>& getSignatures() const;
	const list<Channel>& getChannels(short id) const;
	const Compartment& getCompartment(short id) const;
	const UseExpression& getUseExpression(short id) const;
	//const Mixture& getMixture(small_id id) const;
	const list<Mixture*>& getMixtures() const;
	const list<Mixture::Component*>& getComponents() const;
	const list<Mixture::Agent*>& getAgentPatterns(small_id id) const;
	const vector<simulation::Rule>& getRules() const;
	const vector<simulation::Perturbation*>& getPerts() const;
	const list<state::Variable*>& getObservables() const;


	const Compartment& getCompartmentByCellId(unsigned id) const;

	short idOfAlg(const string& name) const;
	short getVarId(const string &name) const;
	short getVarId(const grammar::ast::Id &name) const;
	short getChannelId(const string &name) const;
	short getCompartmentId(const string &name) const;
	short getSignatureId(const string &name) const;
	unsigned getTokenId(const string &name) const;

	state::BaseExpression* getVarExpression(const string &name) const;

	const DepSet& getDependencies(const Dependency& dep) const;
	const Dependencies& getDependencies() const;
	void addDependency(Dependency key,Dependency::Dep dep,unsigned id);

	template <typename T>
	size_t size() const;
	template <typename T>
	void reserve(size_t count);


	//DEBUG methods
	std::string cellIdToString(unsigned int cell_id) const;
	void show() const;

};

//using env = Environment::getInstance();

} /* namespace pattern */

#endif /* PATTERN_ENVIRONMENT_H_ */
