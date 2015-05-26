/**********************************************
  CS415  Project 3
  Spring  2015
  Student version
 **********************************************/

#include <stdio.h>
#include "deptest.h"


void emitFoundTrueDependenceWithDistance(char *arrayName, int distance)
{
    sprintf(CommentBuffer, " <<< Found TRUE dependence on variable \"%s\" with distance %d >>>",
            arrayName, distance);
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n", CommentBuffer);
}

void emitAssumeTrueDependence(char *arrayName)
{
    sprintf(CommentBuffer, " <<< Assume TRUE dependence on variable \"%s\" with unknown distance >>>",
            arrayName);
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n", CommentBuffer);
}

void emitFoundOutputDependence(char *arrayName)
{
    sprintf(CommentBuffer, " <<< Found OUTPUT dependence on variable \"%s\" >>>",
            arrayName);
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n", CommentBuffer);
}

void emitAssumeOutputDependence(char *arrayName)
{
    sprintf(CommentBuffer, " <<< Assume OUTPUT dependence on variable \"%s\" >>>",
            arrayName);
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n", CommentBuffer);
}

void emitFoundNoDependenciesAndWillVectorize()
{
    sprintf(CommentBuffer, " <<< Found NO dependencies: VECTORIZE >>>");
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n\n", CommentBuffer);
}

void emitFoundDependenciesAndWillNotVectorize()
{
    sprintf(CommentBuffer, " <<< Found at least one dependence: CANNOT VECTORIZE >>>");
    emitComment(CommentBuffer);
    fprintf(depstats, "%s\n\n", CommentBuffer);
}
