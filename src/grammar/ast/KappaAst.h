/*
 * KappaAst.h
 *
 *  Created on: Jan 27, 2016
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_KAPPAAST_H_
#define GRAMMAR_AST_KAPPAAST_H_

#include <list>
#include <vector>
#include <utility>  //std::pair

#include "Statements.h"
#include "../../simulation/Simulation.h"

using namespace std;

namespace grammar {

/** \namespace grammar::ast
 * \brief Abstract Syntax Tree classes */
namespace ast {

/** \brief Store the abstract syntax tree of the model written in Kappa.
 *
 * Contains several list of syntax trees that represent the model
 * read by the parser from kappa files. Evaluate methods in this class
 * produce the initial state of the system and the mappings of
 * ids and names.*/
class KappaAst {

	//AST structures
	list<Declaration> variables,constants;
	list<Agent> signatures;
	list<Id> tokens;
	list<Compartment> compartments;
	list<Channel> channels;
	list<Init> inits;
	list<const Use*> useExpressions;
	list<Rule> rules;
	list<const Pert*> perturbations;

public:
	KappaAst();
	virtual ~KappaAst();

	// evaluation of AST nodes
	vector<float> evaluateTokens(pattern::Environment &env);
	void evaluateSignatures(pattern::Environment &env,const vector<Variable*> &consts);
	vector<Variable*> evaluateDeclarations(pattern::Environment &env,vector<Variable*> &vars,bool is_const = false);
	void evaluateCompartments(pattern::Environment &env,const vector<Variable*>& vars);
	void evaluateUseExpressions(pattern::Environment &env,const vector<Variable*>& consts);
	void evaluateChannels(pattern::Environment &env,const vector<Variable*> &vars);
	void evaluateInits(pattern::Environment &env,const vector<Variable*> &,simulation::Simulation &sim);
	void evaluateRules(pattern::Environment &env,vector<Variable*> &vars);
	void evaluatePerts(pattern::Environment &env,vector<Variable*> &vars);

	//ADD functions
	void add(const Declaration &d);
	void add(const Agent &a);
	void add(const Compartment &c);
	void add(const Channel &c);
	void add(const Id &t);
	void add(const Init &i);
	void add(const Use *u);
	void add(const Rule &r);
	void add(const Pert *p);


	/** \brief Print the AST on stdout.
	 *
	 * \deprecated This method is just for debugging and will be removed. */
	void show();

	/** \brief Evaluate global properties from AST.
	 *
	 * Evaluate global properties (for all compartments) to been distributed among MPI processes.
	 * Return an object of class Environment with mappings of compartment, link and use instructions.
	 */
	//pattern::Environment evaluateGlobals();

	/** \brief Evaluate local properties from AST.
	 *
	 * Evaluate all local properties and variables of this compartment
	 * to return an object of class Environment with all mappings needed by the state of
	 * system.
	 */
	//pattern::Environment evaluateEnvironment(pattern::Environment globs, int comp_id);

	/** \brief Initialize SiteGraph and all structures to create the state of system.
	 *
	 * Evaluate the initial state of this compartment from AST, and return it in an object of
	 * class State.
	 */
	//state::State evaluateState(pattern::Environment env, int comp_id);


};

} /* namespace ast */
}

#endif /* GRAMMAR_AST_KAPPAAST_H_ */
