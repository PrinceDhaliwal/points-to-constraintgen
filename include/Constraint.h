/**
 * \file Constraint.h
 * defines the class ptsto::Constraint
 * 
 * Different LLVM IR statements have been divided into five basic
 * points-to constraint types. All these types have rhs and lhs (or 
 * source and destination).
 */
#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include "PointsToNode.h"
#include <llvm/Support/raw_ostream.h>
#include <limits>

namespace ptsto {

class PointerSymbolTable;

/**
 * the various types of assignments that can be 
 * present in the IR and correspond a class of constraints
 */
enum class ConstraintType {
    kInvalid,
    kAddressOf, /* e.g. %a = alloca %type, or in C, a = &b */
    kLoad,      /* e.g. %a = load %type*, %type** %b,  in C as `a = *b` */
    kStore,     /* e.g. store %type* %b, %type** %a,   in C as `*a = b` */
    kCopy,      /* e.g. %a = getelementptr %type, %type* %b, in C as `a = b` */
    kReturn
};

/**
 * holds the information about a constraint for points-to analysis.
 *
 * It stores the pointers to symbol table entry of source and
 * destination. It also stores the type of constraint which can be
 * one of ptsto::ConstraintType.
 */
class Constraint {
public:
    /**
     * constructor
     * \param dest the destination node i.e. lhs
     * \param source the source node i.e. rhs
     * \param type type of the constraint i.e. one of ConstraintType
     */
    Constraint(NodeIndex dest, NodeIndex source, ConstraintType type)
        : type_{ type }, source_{ source }, dest_{ dest }
    { }

    /**
     * returns id of the destination node. Useful for debugging and
     * printing
     *
     * \return id of the destination node
     */
    NodeIndex getDestination() const {
        return dest_;
    }

    /**
     * returns the id of the source node. Useful for debugging and printing
     *
     * \return id of the source node
     */
    NodeIndex getSource() const {
        return source_;
    }

    /**
     * \return type of the constraint
     */
    ConstraintType getType() const {
        return type_;
    }

    /**
     * Useful for debugging using gdb. It dumps the constraint to os
     *
     * \param os llvm::raw_ostream object
     */
    void dump(llvm::raw_ostream &os) const;

    /**
     * More useful method for getting the pointer to the LHS node directly
     * \returns pointer to the PointsToNode object
     */
    PointsToNode *getLHSNode();
    PointsToNode *getLHSNode() const;

    /**
     * More useful method for getting the pointer to the RHS node directly
     * \returns pointer to the PointsToNode object
     */
    PointsToNode *getRHSNode();
    PointsToNode *getRHSNode() const;
private:
    ConstraintType type_;
    NodeIndex source_;
    NodeIndex dest_;
};

/**
 * operator< defined for storing the constraint in a map
 */
static inline bool operator<(const Constraint &lhs, const Constraint &rhs) {
    return lhs.getSource() < rhs.getSource()
                || lhs.getDestination() < rhs.getDestination();
}

/**
 * To compare two Constraint objects
 */
static inline bool operator==(const Constraint &lhs, const Constraint &rhs) {
    return lhs.getSource() == rhs.getSource()
            && lhs.getDestination() == rhs.getDestination()
            && lhs.getType() == rhs.getType();
}

}

#endif
