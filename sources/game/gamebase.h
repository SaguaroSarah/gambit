//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Implementation of base (full explicit representation) games
//
// This file is part of Gambit
// Copyright (c) 2003, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#ifndef GAMEBASE_H
#define GAMEBASE_H

#include "nfgcont.h"
#include "mixed.h"
#include "behav.h"

//
// Forward declarations
//
class gbtGame;
class gbtGameActionBase;
class gbtGameInfosetBase;
class gbtGameNodeBase;
class gbtGamePlayerBase;
class gbtGameBase;

class gbtGameOutcomeBase : public gbtGameOutcomeRep {
public:
  int m_id;
  gbtText m_label;
  gbtBlock<gbtNumber> m_payoffs;

  gbtGameOutcomeBase(gbtGameBase *, int);

  int GetId(void) const { return m_id; }
  gbtText GetLabel(void) const { return m_label; }
  void SetLabel(const gbtText &p_label) { m_label = p_label; }

  gbtArray<gbtNumber> GetPayoff(void) const { return m_payoffs; }
  gbtNumber GetPayoff(const gbtGamePlayer &p_player) const
  { return m_payoffs[p_player->GetId()]; }
  void SetPayoff(const gbtGamePlayer &p_player, const gbtNumber &p_value)
  { m_payoffs[p_player->GetId()] = p_value; } 
};

class gbtGameStrategyBase : public gbtGameStrategyRep {
public:
  int m_id;
  gbtText m_label;
  gbtGamePlayerBase *m_player;
  gbtArray<int> m_actions;
  long m_index;

  gbtGameStrategyBase(int p_id, gbtGamePlayerBase *p_player,
		      const gbtArray<int> &p_actions)
    : m_id(p_id), m_player(p_player), m_actions(p_actions) { }
  virtual ~gbtGameStrategyBase() { }

  gbtText GetLabel(void) const { return m_label; }
  void SetLabel(const gbtText &p_label) { m_label = p_label; }
  int GetId(void) const { return m_id; }

  gbtGamePlayer GetPlayer(void) const;
  gbtGameAction GetAction(const gbtGameInfoset &p_infoset) const;

  const gbtArray<int> &GetBehavior(void) const { return m_actions; }
  long GetIndex(void) const;
};

class gbtGamePlayerBase : public gbtGamePlayerRep {
public:
  int m_id;
  gbtGameBase *m_efg;
  gbtText m_label;
  gbtBlock<gbtGameInfosetBase *> m_infosets;
  gbtBlock<gbtGameStrategyBase *> m_strategies;

  gbtGamePlayerBase(gbtGameBase *, int);
  virtual ~gbtGamePlayerBase();

  gbtText GetLabel(void) const { return m_label; }
  void SetLabel(const gbtText &p_label) { m_label = p_label; }
  int GetId(void) const { return m_id; }

  bool IsChance(void) const { return (m_id == 0); }

  int NumInfosets(void) const { return m_infosets.Length(); }
  gbtGameInfoset NewInfoset(int p_actions);
  gbtGameInfoset GetInfoset(int p_index) const;

  int NumStrategies(void) const { return m_strategies.Length(); }
  gbtGameStrategy GetStrategy(int p_index) const 
  { return m_strategies[p_index]; }

  gbtNumber GetMinPayoff(void) const;
  gbtNumber GetMaxPayoff(void) const;
};

class gbtGameActionBase : public gbtGameActionRep {
public:
  int m_id;
  gbtGameInfosetBase *m_infoset;
  bool m_deleted;
  gbtText m_label;

  gbtGameActionBase(gbtGameInfosetBase *, int);
  virtual ~gbtGameActionBase() { } 

  gbtText GetLabel(void) const { return m_label; }
  void SetLabel(const gbtText &p_label) { m_label = p_label; }
  int GetId(void) const { return m_id; }
  
  gbtGameInfoset GetInfoset(void) const;
  gbtGamePlayer GetPlayer(void) const;

  gbtNumber GetChanceProb(void) const;
  bool Precedes(gbtGameNode) const;
  
  void DeleteAction(void);
};

class gbtGameInfosetBase : public gbtGameInfosetRep {
public:
  int m_id;
  gbtGamePlayerBase *m_player;
  bool m_deleted;
  gbtText m_label;
  int m_refCount;
  gbtBlock<gbtGameActionBase *> m_actions;
  gbtBlock<gbtNumber> m_chanceProbs;
  gbtBlock<gbtGameNodeBase *> m_members;
  bool m_flag;
  int m_whichbranch;

  gbtGameInfosetBase(gbtGamePlayerBase *, int id, int br);
  virtual ~gbtGameInfosetBase();

  gbtText GetLabel(void) const { return m_label; }
  void SetLabel(const gbtText &p_label) { m_label = p_label; }
  int GetId(void) const { return m_id; }

  void DeleteInfoset(void);

  bool IsChanceInfoset(void) const;

  gbtGamePlayer GetPlayer(void) const;
  void SetPlayer(gbtGamePlayer);
  
  void SetChanceProb(int act, const gbtNumber &value); 
  gbtNumber GetChanceProb(int act) const;

  gbtGameAction InsertAction(int where);

  gbtGameAction GetAction(int act) const;
  int NumActions(void) const;

  gbtGameNode GetMember(int m) const;
  int NumMembers(void) const;

  bool Precedes(gbtGameNode) const;

  void MergeInfoset(gbtGameInfoset from);
  void Reveal(gbtGamePlayer);

  void PrintActions(gbtOutput &) const;
};

class gbtGameNodeBase : public gbtGameNodeRep {
public:
  int m_id;
  gbtGameBase *m_efg;
  bool m_deleted;
  gbtText m_label;
  int m_refCount;

  bool m_mark;
  gbtGameInfosetBase *m_infoset;
  gbtGameNodeBase *m_parent;
  gbtGameOutcomeBase *m_outcome;
  gbtBlock<gbtGameNodeBase *> m_children;
  mutable gbtGameNodeBase *m_whichbranch, *m_ptr;

  gbtGameNodeBase(gbtGameBase *, gbtGameNodeBase *);
  virtual ~gbtGameNodeBase();

  gbtText GetLabel(void) const { return m_label; }
  void SetLabel(const gbtText &p_label) { m_label = p_label; }
  int GetId(void) const { return m_id; }

  int NumChildren(void) const;
  gbtGameNode GetChild(int i) const;
  gbtGameNode GetChild(const gbtGameAction &) const; 
  void GetNodes(gbtList<gbtGameNode> &) const;
  void GetTerminalNodes(gbtList<gbtGameNode> &) const;

  bool IsRoot(void) const { return (m_parent != 0); }
  bool IsPredecessorOf(const gbtGameNode &) const;

  gbtGameNode GetParent(void) const;
  gbtGameAction GetPriorAction(void) const; // returns null if root node

  gbtGameInfoset GetInfoset(void) const;
  int GetMemberId(void) const;

  gbtGamePlayer GetPlayer(void) const;

  bool IsSubgameRoot(void) const;

  gbtGameOutcome GetOutcome(void) const;
  void SetOutcome(const gbtGameOutcome &);

  gbtGameNode InsertMove(gbtGameInfoset);
  // Note: Starting in 0.97.1.1, this now deletes the *parent* move
  void DeleteMove(void);
  void DeleteTree(void);

  void JoinInfoset(gbtGameInfoset);
  gbtGameInfoset LeaveInfoset(void);

  void DeleteOutcome(gbtGameOutcomeBase *outc);

  gbtGameNode GetPriorSibling(void) const;
  gbtGameNode GetNextSibling(void) const;

  gbtGameNode GetPriorMember(void) const;
  gbtGameNode GetNextMember(void) const;

  void MarkSubtree(bool p_mark);
};

//
// Need to think more carefully about this factoring:
// goal is to eventually again have a normal form game where
// outcomes are identified one-to-one with contingencies,
// and where explicit outcome representations are not needed internally.
//
class gbtConstGameBase : public virtual gbtGameObject {
public:
  gbtText m_label, m_comment;
  gbtBlock<gbtGamePlayerBase *> m_players;
  gbtBlock<gbtGameOutcomeBase *> m_outcomes;
};

class gbtGameBase : public gbtConstGameBase, public gbtGameRep {
public:
  bool sortisets;
  int m_numNodes;
  gbtBlock<gbtGameOutcomeBase *> m_results;
  gbtGameNodeBase *root;
  gbtGamePlayerBase *chance;

  // Create a trivial tree representation game
  gbtGameBase(void);
  // Create a matrix game
  gbtGameBase(const gbtArray<int> &);
  ~gbtGameBase();

  // this is for use with the copy constructor
  void CopySubtree(gbtGameBase *,
		   gbtGameNodeBase *, gbtGameNodeBase *);

  void CopySubtree(gbtGameNodeBase *, gbtGameNodeBase *,
		   gbtGameNodeBase *);

  gbtGameOutcome NewOutcome(int index);

  void WriteEfg(gbtOutput &, gbtGameNodeBase *) const;

  void Payoff(gbtGameNodeBase *n, gbtNumber,
	      const gbtPVector<int> &, gbtVector<gbtNumber> &) const;
  void Payoff(gbtGameNodeBase *n, gbtNumber,
	      const gbtArray<gbtArray<int> > &, gbtArray<gbtNumber> &) const;
  
  void InfosetProbs(gbtGameNodeBase *n, gbtNumber,
		    const gbtPVector<int> &, gbtPVector<gbtNumber> &) const;
    

  void IndexStrategies(void);
  void SortInfosets(void);
  void NumberNodes(gbtGameNodeBase *, int &);
  void ComputeReducedStrategies(void);

  void InsertMove(gbtGameNodeBase *, gbtGameInfosetBase *);
  void DeleteMove(gbtGameNodeBase *);
  void DeleteTree(gbtGameNodeBase *);

  gbtGameInfosetBase *NewInfoset(gbtGamePlayerBase *,
				  int p_id, int p_actions);
  void DeleteInfoset(gbtGameInfosetBase *);
  void JoinInfoset(gbtGameInfosetBase *, gbtGameNodeBase *); 
  gbtGameInfosetBase *LeaveInfoset(gbtGameNodeBase *);
  void MergeInfoset(gbtGameInfosetBase *, gbtGameInfosetBase *);
  void Reveal(gbtGameInfosetBase *, gbtGamePlayerBase *);
  void SetPlayer(gbtGameInfosetBase *, gbtGamePlayerBase *);

  void DeleteAction(gbtGameInfosetBase *, gbtGameActionBase *);

  void DeleteOutcome(gbtGameOutcome p_outcome);

  // Utility routines for subgames
  void MarkTree(const gbtGameNodeBase *, const gbtGameNodeBase *);
  bool CheckTree(const gbtGameNodeBase *, const gbtGameNodeBase *);
  void MarkSubgame(gbtGameNodeBase *, gbtGameNodeBase *);

  // Formerly the copy constructor
  gbtGame Copy(gbtGameNode = gbtGameNode(0)) const;
  
  // TITLE ACCESS AND MANIPULATION
  void SetLabel(const gbtText &s);
  gbtText GetLabel(void) const;
  
  void SetComment(const gbtText &);
  gbtText GetComment(void) const;

  // WRITING DATA FILES
  void WriteEfg(gbtOutput &p_file) const;
  void WriteNfg(gbtOutput &p_file) const;

  // DATA ACCESS -- GENERAL INFORMATION
  bool IsTree(void) const { return (root != 0); }
  bool IsMatrix(void) const { return (root == 0); }

  bool IsConstSum(void) const; 
  bool IsPerfectRecall(void) const;
  bool IsPerfectRecall(gbtGameInfoset &, gbtGameInfoset &) const;
  long RevisionNumber(void) const;
  gbtNumber GetMinPayoff(void) const;
  gbtNumber GetMaxPayoff(void) const;
 
  // DATA ACCESS -- NODES
  int NumNodes(void) const;
  gbtGameNode GetRoot(void) const;
  gbtList<gbtGameNode> GetNodes(void) const;
  gbtList<gbtGameNode> GetTerminalNodes(void) const;

  // DATA ACCESS -- PLAYERS
  int NumPlayers(void) const;
  gbtGamePlayer GetChance(void) const;
  gbtGamePlayer NewPlayer(void);
  gbtGamePlayer GetPlayer(int index) const;

  // DATA ACCESS -- OUTCOMES
  int NumOutcomes(void) const;
  gbtGameOutcome GetOutcome(int p_id) const;
  gbtGameOutcome NewOutcome(void);

  // DATA ACCESS -- SUPPORTS
  gbtEfgSupport NewEfgSupport(void) const;
  gbtNfgSupport NewNfgSupport(void) const;

  gbtNfgContingency NewContingency(void) const;

  // DATA ACCESS -- PROFILES
  gbtMixedProfile<double> NewMixedProfile(double) const;
  gbtMixedProfile<gbtRational> NewMixedProfile(const gbtRational &) const;
  gbtMixedProfile<gbtNumber> NewMixedProfile(const gbtNumber &) const;

  gbtBehavProfile<double> NewBehavProfile(double) const;
  gbtBehavProfile<gbtRational> NewBehavProfile(const gbtRational &) const;
  gbtBehavProfile<gbtNumber> NewBehavProfile(const gbtNumber &) const;

  // EDITING OPERATIONS
  void DeleteEmptyInfosets(void);

  gbtGameNode CopyTree(gbtGameNode src, gbtGameNode dest);
  gbtGameNode MoveTree(gbtGameNode src, gbtGameNode dest);

  gbtGameAction InsertAction(gbtGameInfoset);
  gbtGameAction InsertAction(gbtGameInfoset, const gbtGameAction &at);

  void SetChanceProb(gbtGameInfoset, int, const gbtNumber &);

  void MarkSubgames(void);

  int BehavProfileLength(void) const;
  int MixedProfileLength(void) const;
  int TotalNumInfosets(void) const;

  gbtArray<int> NumInfosets(void) const;  // Does not include chance infosets
  int NumPlayerInfosets(void) const;
  gbtPVector<int> NumActions(void) const;
  int NumPlayerActions(void) const;
  gbtPVector<int> NumMembers(void) const;
  gbtArray<int> NumStrategies(void) const;
  int NumStrats(int) const;
  
  // COMPUTING VALUES OF PROFILES
  void Payoff(const gbtPVector<int> &profile,
		      gbtVector<gbtNumber> &payoff) const;
  void Payoff(const gbtArray<gbtArray<int> > &profile,
		      gbtArray<gbtNumber> &payoff) const;

  void InfosetProbs(const gbtPVector<int> &profile,
			    gbtPVector<gbtNumber> &prob) const;
};

class gbtNfgContingencyTree : public gbtNfgContingencyRep {
private:
  gbtGameBase *m_nfg;
  gbtArray<gbtGameStrategy> m_profile;
  
public:
  gbtNfgContingencyTree(gbtGameBase *);
  virtual ~gbtNfgContingencyTree() { }
  
  virtual gbtNfgContingencyRep *Copy(void) const;

  gbtGameStrategy GetStrategy(const gbtGamePlayer &p_player) const
    { return m_profile[p_player->GetId()]; }
  void SetStrategy(gbtGameStrategy);

  void SetOutcome(const gbtGameOutcome &) const;
  gbtGameOutcome GetOutcome(void) const;

  gbtNumber GetPayoff(const gbtGamePlayer &) const;
};

class gbtNfgContingencyTable : public gbtNfgContingencyRep {
private:
  gbtGameBase *m_nfg; 
  long m_index;
  gbtArray<gbtGameStrategy> m_profile;
  
public:
  gbtNfgContingencyTable(gbtGameBase *);
  virtual ~gbtNfgContingencyTable() { }
  
  virtual gbtNfgContingencyRep *Copy(void) const;

  gbtGameStrategy GetStrategy(const gbtGamePlayer &p_player) const
    { return m_profile[p_player->GetId()]; }
  void SetStrategy(gbtGameStrategy);

  void SetOutcome(const gbtGameOutcome &) const;
  gbtGameOutcome GetOutcome(void) const;

  gbtNumber GetPayoff(const gbtGamePlayer &) const;
};


class gbtNfgSupportBase : public gbtNfgSupportRep {
public:
  gbtGameBase *m_nfg;
  // This really could be a gbtPVector<bool> probably, but we'll keep
  // it this way for now to placate possibly older compilers.
  gbtPVector<int> m_strategies;
  gbtText m_label;
  
  bool Undominated(gbtNfgSupportBase &newS, int pl, bool strong,
		   gbtOutput &tracefile, gbtStatus &status) const;

  // LIFECYCLE
  gbtNfgSupportBase(gbtGameBase *);
  ~gbtNfgSupportBase() { }

  // OPERATORS
  // This acts as the assignment operator and copy constructor
  gbtNfgSupportRep *Copy(void) const;
  
  bool operator==(const gbtNfgSupportRep &) const;
  bool operator!=(const gbtNfgSupportRep &p_support) const
  { return !(*this == p_support); }

  // DATA ACCESS: GENERAL
  gbtText GetLabel(void) const { return m_label; }
  void SetLabel(const gbtText &p_label) { m_label = p_label; }
  
  // DATA ACCESS: STRATEGIES
  int NumStrats(int pl) const;
  int NumStrats(const gbtGamePlayer &p_player) const 
    { return NumStrats(p_player->GetId()); }
  gbtArray<int> NumStrategies(void) const;
  int MixedProfileLength(void) const;

  int GetIndex(gbtGameStrategy) const;
  bool Contains(gbtGameStrategy) const;

  // DATA ACCESS -- PROFILES
  gbtMixedProfile<double> NewMixedProfile(double) const;
  gbtMixedProfile<gbtRational> NewMixedProfile(const gbtRational &) const;
  gbtMixedProfile<gbtNumber> NewMixedProfile(const gbtNumber &) const;

  // MANIPULATION
  void AddStrategy(gbtGameStrategy);
  void RemoveStrategy(gbtGameStrategy);
  
  // DOMINANCE AND ELIMINATION OF STRATEGIES
  bool Dominates(gbtGameStrategy, gbtGameStrategy, bool strong) const;
  bool IsDominated(gbtGameStrategy, bool strong) const; 

  gbtNfgSupport Undominated(bool strong, const gbtArray<int> &players,
			    gbtOutput &tracefile, gbtStatus &status) const;
  gbtNfgSupport MixedUndominated(bool strong, gbtPrecision precision,
				 const gbtArray<int> &players,
				 gbtOutput &, gbtStatus &status) const;

  // OUTPUT
  void Output(gbtOutput &) const;

  // IMPLEMENTATION OF gbtConstGameRep INTERFACE
  bool IsTree(void) const { return m_nfg->IsTree(); }
  bool IsMatrix(void) const { return m_nfg->IsMatrix(); }
  
  int NumPlayers(void) const { return m_nfg->NumPlayers(); }
  gbtGamePlayer GetPlayer(int index) const; 
  
  int NumOutcomes(void) const { return m_nfg->NumOutcomes(); }
  gbtGameOutcome GetOutcome(int index) const 
  { return m_nfg->GetOutcome(index); }

  bool IsConstSum(void) const { return m_nfg->IsConstSum(); }
  gbtNumber GetMaxPayoff(void) const { return m_nfg->GetMaxPayoff(); }
  gbtNumber GetMinPayoff(void) const { return m_nfg->GetMinPayoff(); }

  // The following are just echoed from the base game.  In the future,
  // derivation from gbtGame will handle these.
  gbtText GetComment(void) const { return m_nfg->GetComment(); }
  void SetComment(const gbtText &p_comment) { m_nfg->SetComment(p_comment); }

  gbtNfgSupport NewNfgSupport(void) const { return Copy(); }
  gbtNfgContingency NewContingency(void) const;

};

class gbtEfgSupportPlayer;

class gbtEfgSupportBase : public gbtEfgSupportRep {
public:
  gbtText m_label;
  gbtGameBase *m_efg;
  gbtArray<gbtEfgSupportPlayer *> m_players;

  // The following members were added from a derived class.
  // The proper factoring of these features is uncertain...
  gbtArray<gbtList<bool> >         is_infoset_active;
  gbtArray<gbtList<gbtList<bool> > > is_nonterminal_node_active;

  void InitializeActiveListsToAllActive();
  void InitializeActiveListsToAllInactive();
  void InitializeActiveLists();

  bool infoset_has_active_nodes(const int pl, const int iset) const;
  bool infoset_has_active_nodes(const gbtGameInfoset &) const;
  void activate_this_and_lower_nodes(const gbtGameNode &);
  void deactivate_this_and_lower_nodes(const gbtGameNode &);

  void activate(const gbtGameNode &);
  void deactivate(const gbtGameNode &);
  void activate(const gbtGameInfoset &);
  void deactivate(const gbtGameInfoset &);

  void deactivate_this_and_lower_nodes_returning_deactivated_infosets(
                                                 const gbtGameNode &,
						 gbtList<gbtGameInfoset> *);

  gbtEfgSupportBase(gbtGameBase *);
  gbtEfgSupportBase(const gbtEfgSupportBase &);
  virtual ~gbtEfgSupportBase();

  gbtEfgSupportRep *Copy(void) const;

  bool operator==(const gbtEfgSupportRep &) const;

  void SetLabel(const gbtText &p_label) { m_label = p_label; }

  gbtGame GetTree(void) const { return m_efg; }

  int NumDegreesOfFreedom(void) const;

  // Checks to see that every infoset in the support has at least one
  // action in it.
  bool HasActiveActionAt(const gbtGameInfoset &) const;
  bool HasActiveActionsAtAllInfosets(void) const;

  bool Contains(const gbtGameAction &) const;
  bool Contains(int pl, int iset, int act) const;
  int GetIndex(const gbtGameAction &) const;
  gbtGameAction GetAction(const gbtGameInfoset &, int index) const;
  gbtGameAction GetAction(int pl, int iset, int index) const;

  // Action editing functions
  void AddAction(const gbtGameAction &);
  bool RemoveAction(const gbtGameAction &);

  // Number of Sequences for the player
  int NumSequences(int pl) const;
  int TotalNumSequences(void) const;

  // Reachable Nodes and Information Sets
  gbtList<gbtGameNode> ReachableNonterminalNodes(const gbtGameNode &) const;
  gbtList<gbtGameNode> ReachableNonterminalNodes(const gbtGameNode &,
					      const gbtGameAction &) const;
  gbtList<gbtGameInfoset> ReachableInfosets(const gbtGameNode &) const;
  gbtList<gbtGameInfoset> ReachableInfosets(const gbtGameNode &,
					 const gbtGameAction &) const;
  gbtList<gbtGameInfoset> ReachableInfosets(const gbtGamePlayer &) const;

  bool AlwaysReaches(const gbtGameInfoset &) const;
  bool AlwaysReachesFrom(const gbtGameInfoset &, const gbtGameNode &) const;
  bool MayReach(const gbtGameNode &) const;
  bool MayReach(const gbtGameInfoset &) const;

  bool Dominates(const gbtGameAction &, const gbtGameAction &,
		 bool strong, bool conditional) const;
  bool IsDominated(const gbtGameAction &,
		   bool strong, bool conditional) const;
  gbtEfgSupport Undominated(bool strong, bool conditional,
			    const gbtArray<int> &players,
			    gbtOutput &, // tracefile 
			    gbtStatus &status) const;


  // IMPLEMENTATION OF gbtConstGameRep INTERFACE
  bool IsTree(void) const { return true; }
  bool IsMatrix(void) const { return false; }

  gbtText GetLabel(void) const { return m_efg->GetLabel(); }
  gbtText GetComment(void) const { return ""; }

  int NumPlayers(void) const { return m_efg->NumPlayers(); }
  gbtGamePlayer GetPlayer(int index) const { return m_efg->GetPlayer(index); }

  int NumOutcomes(void) const { return m_efg->NumOutcomes(); }
  gbtGameOutcome GetOutcome(int index) const 
  { return m_efg->GetOutcome(index); }

  bool IsConstSum(void) const { return m_efg->IsConstSum(); }
  gbtNumber GetMaxPayoff(void) const { return m_efg->GetMaxPayoff(); }
  gbtNumber GetMinPayoff(void) const { return m_efg->GetMinPayoff(); }

  // IMPLEMENTATION OF gbtConstEfgRep INTERFACE

  // DATA ACCESS -- GENERAL
  bool IsPerfectRecall(void) const { return m_efg->IsPerfectRecall(); }

  // DATA ACCESS -- NODES
  int NumNodes(void) const { return m_efg->NumNodes(); }
  gbtGameNode GetRoot(void) const { return m_efg->GetRoot(); }
  gbtList<gbtGameNode> GetNodes(void) const
  { return m_efg->GetNodes(); }
  gbtList<gbtGameNode> GetTerminalNodes(void) const
  { return m_efg->GetTerminalNodes(); }

  // DATA ACCESS -- ACTIONS
  gbtPVector<int> NumActions(void) const;
  int BehavProfileLength(void) const;

  // DATA ACCESS -- INFORMATION SETS
  int TotalNumInfosets(void) const { return m_efg->TotalNumInfosets(); }
  gbtArray<int> NumInfosets(void) const { return m_efg->NumInfosets(); }
  int NumPlayerInfosets(void) const { return m_efg->NumPlayerInfosets(); }
  int NumPlayerActions(void) const { return m_efg->NumPlayerActions(); }
  gbtPVector<int> NumMembers(void) const { return m_efg->NumMembers(); }

  // DATA ACCESS -- SUPPORTS
  gbtEfgSupport NewEfgSupport(void) const;

  // DATA ACCESS -- PROFILES
  gbtBehavProfile<double> NewBehavProfile(double) const;
  gbtBehavProfile<gbtRational> NewBehavProfile(const gbtRational &) const;
  gbtBehavProfile<gbtNumber> NewBehavProfile(const gbtNumber &) const;

  void SetComment(const gbtText &p_comment) { m_efg->SetComment(p_comment); }
  gbtGamePlayer GetChance(void) const { return m_efg->GetChance(); }

  void Dump(gbtOutput &) const;

  // The subsequent members were merged from a derived class.

  // Find the reachable nodes at an infoset
  gbtList<gbtGameNode> ReachableNodesInInfoset(const gbtGameInfoset &) const;

  bool HasActiveActionsAtActiveInfosets(void);
  bool HasActiveActionsAtActiveInfosetsAndNoOthers(void);

  bool InfosetIsActive(const gbtGameInfoset &) const;

  bool RemoveActionReturningDeletedInfosets(const gbtGameAction &,
					    gbtList<gbtGameInfoset> *);
  // Information
  bool InfosetIsActive(const int pl, const int iset) const;
  int  NumActiveNodes(const int pl, const int iset) const;
  int  NumActiveNodes(const gbtGameInfoset &) const;
  bool NodeIsActive(const int pl, const int iset, const int node) const;
  bool NodeIsActive(const gbtGameNode &) const;

  gbtList<gbtGameNode> ReachableNonterminalNodes() const;
};

#endif  // GAMEBASE_H
