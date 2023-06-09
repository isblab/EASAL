/*
 This file is part of EASAL.

 EASAL is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 EASAL is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * Algorithm.cpp
 *
 *  Created on: 2014
 *      Author: Aysegul Ozkan
 */

#include "ConvexChart.h"

#include <math.h>
#include <time.h>

#include <cstdlib>
#include <iomanip>  // std::setw
#include <iostream>
#include <queue>
#include <string>

#include "ActiveConstraintGraph.h"
#include "MolecularUnit.h"
#include "Orientation.h"
#include "Settings.h"
using namespace std;

#define PI 3.14159265

ConvexChart::ConvexChart(ActiveConstraintGraph* cgk, bool dense,
                         CayleyParameterization* cayleyParam,
                         PredefinedInteractions* df) {
  Settings* sett = Settings::getInstance();
  if (sett->Sampling.JacobianSampling) {
    pr1 = (int)round(2.0 * sett->Sampling.GridXY / sett->Sampling.gridSteps[0]);
    pr2 = (int)round(2.0 * sett->Sampling.GridXY / sett->Sampling.gridSteps[1]);
    pr3 = (int)round(2.0 * sett->Sampling.GridXY / sett->Sampling.gridSteps[2]);
    pr4 = 38;
    pr5 = 10;
    pr6 = 38;

    this->visited = new bool*****[pr1];

    for (size_t i = 0; i < pr1; i++) {
      this->visited[i] = new bool****[pr2];
    }

    for (size_t i = 0; i < pr1; i++) {
      for (size_t j = 0; j < pr2; j++) {
        this->visited[i][j] = new bool***[pr3];
      }
    }

    for (size_t i = 0; i < pr1; i++) {
      for (size_t j = 0; j < pr2; j++) {
        for (size_t k = 0; k < pr3; k++) {
          this->visited[i][j][k] = new bool**[pr4];
        }
      }
    }

    for (size_t i = 0; i < pr1; i++) {
      for (size_t j = 0; j < pr2; j++) {
        for (size_t k = 0; k < pr3; k++) {
          for (size_t l = 0; l < pr4; l++) {
            this->visited[i][j][k][l] = new bool*[pr5];
          }
        }
      }
    }

    for (size_t i = 0; i < pr1; i++) {
      for (size_t j = 0; j < pr2; j++) {
        for (size_t k = 0; k < pr3; k++) {
          for (size_t l = 0; l < pr4; l++) {
            for (size_t m = 0; m < pr5; m++) {
              this->visited[i][j][k][l][m] = new bool[pr6];
            }
          }
        }
      }
    }

    for (size_t i = 0; i < pr1; i++)
      for (size_t j = 0; j < pr2; j++)
        for (size_t k = 0; k < pr3; k++)
          for (size_t l = 0; l < pr4; l++)
            for (size_t m = 0; m < pr5; m++)
              for (size_t n = 0; n < pr6; n++)
                this->visited[i][j][k][l][m][n] = false;

    for (size_t i = 0; i < 6; i++) this->direction[i] = 1;
  }

  for (size_t i = 0; i < 6; i++) {
    this->minOfMin[i] = 0;
    this->maxOfMax[i] = 0;
  }

  this->currentGraph = cgk;

  this->helA = sett->runTimeObjects.muA;
  this->helB = sett->runTimeObjects.muB;
  this->df = df;

  verticesA = this->currentGraph->getVerticesA();
  verticesB = this->currentGraph->getVerticesB();

  contacts = this->currentGraph->getContacts();

  dir = -1;
  bin = 1;

  this->dense = dense;
  for (int i = 0; i < 6; i++) this->point[i] = 0;

  for (int i = 0; i < NO; i++)
    for (int j = 0; j < NO; j++) {
      this->param_lengthUpper[i][j] = -1;
      this->param_lengthLower[i][j] = -1;
      this->edge_length[i][j] = -1;
      this->param_length[i][j] = -1;
    }

  this->stepSize = this->currentGraph->getStepSize();
  if (dense) {
    this->stepSize = this->stepSize / 2;
    this->currentGraph->setStepSize(this->stepSize);
  }

  this->parameters = cayleyParam->getParameters();

  this->updateList = cayleyParam->getUpdateList();
  this->boundaryComputationWay = cayleyParam->getBoundaryComputationWay();
  this->partial3tree = cayleyParam->is_partial3tree();
  this->tetrahedra = cayleyParam->getTetras();

  computeFixEdgeDistances();
  stepSize_of_stepGridContact = this->stepSize;
}

ConvexChart::~ConvexChart() {}

void ConvexChart::computeFixEdgeDistances() {
  Settings* sett = Settings::getInstance();
  for (size_t i = 0; i < this->verticesA.size(); i++) {  // inside helix a
    for (size_t j = i + 1; j < this->verticesA.size(); j++) {
      this->edge_length[i][j] = Utils::dist(
          helA->getAtomAt(verticesA[i])->getLocation(),
          helA->getAtomAt(verticesA[j])
              ->getLocation());  // The location retrieved from an atom is the
                                 // actual pointer don't delete it
      setMinMaxParam(i, j);
    }
  }

  for (size_t i = 0; i < this->verticesB.size(); i++) {  // inside helix b
    for (size_t j = i + 1; j < this->verticesB.size(); j++) {
      this->edge_length[i + NO / 2][j + NO / 2] = Utils::dist(
          helB->getAtomAt(verticesB[i])->getLocation(),
          helB->getAtomAt(verticesB[j])
              ->getLocation());  // The location retrieved from an atom is the
                                 // actual pointer don't delete it
      setMinMaxParam(i + NO / 2, j + NO / 2);
    }
  }

  if (!sett->Sampling.short_range_sampling) {
    for (size_t i = 0; i < this->contacts.size();
         i++)  // between the contacts in helixA and helixB
    {
      int v1 = this->contacts[i].first;
      int v2 = this->contacts[i].second;
      this->edge_length[v2][v1] = df->sumOfRadii(
          getAtom(v1),
          getAtom(v2));  //+ sett->Collision::contactUpperThreshold ; //
                         //*sett->Collision::contactLowerThreshold ;//
      setMinMaxParam(v2, v1);
    }
  }
}

bool ConvexChart::initializeChart(bool contin, ActiveConstraintRegion* region) {
  Settings* sett = Settings::getInstance();
  // this->dense = dense;
  // if(dense){
  //		this->stepSize = this->stepSize/2;
  //		this->currentGraph->setStepSize(this->stepSize);
  //	}

  // if you change the order of the parameters, min-max range can change ??
  for (int i = this->parameters.size() - 1; i >= 0;
       i--) {  // to start from reverse if you put contacts at the end
    int v1 = this->parameters[i].first;
    int v2 = this->parameters[i].second;

    int isContact = find_pair_in_vector(this->contacts, this->parameters[i]);
    if (isContact >= 0 && sett->Sampling.short_range_sampling) {
      this->param_lengthUpper[v1][v2] =
          df->bondingUpperBound(getAtom(v1), getAtom(v2));
      this->param_lengthLower[v1][v2] =
          df->bondingLowerBound(getAtom(v1), getAtom(v2));
    } else {
      computeRange(v1, v2, i);
    }

    this->maxOfMax[i] = this->param_lengthUpper[v1][v2];
    this->minOfMin[i] = this->param_lengthLower[v1][v2];

    if (region->GetSamplePointsCount() == 0 || contin) {
      if (region->GetWitnessPoints()
              .empty())  // for initial dumbbell  or for a dumbbell which in
                         // fact has a witness point but not saved to its file
                         // (since we do save the witness only one leaf node)
        this->witnessPoint[i] = this->param_lengthLower[v1][v2];
      //			else       //COMMENDED BECAUSE i ADD
      //desc->setWitnessPoint(wp4); TO ALGORITHM 				this->witnessPoint[i] =
      //(*(this->currentGraph->getSpace().front()))[i]; //start from the
      //middle(witness) point

      if (contin) {
        if (region->GetSpaceSize() >
            0)  // space + witness not empty  //getSpace().empty()
        {
          this->point[i] =
              (*(region->getSpace()
                     .back()))[i];  // CONTINUE FROM LAST POINT BUT KEEP THE
                                    // ORIGINAL MIDPOINT AT THE SAME TIME.
          if (region->GetWitnessPointsCount() > 0)
            this->witnessPoint[i] =
                (*(region->GetWitnessPoints()
                       .front()))[i];  // start from the middle(witness) point
        } else
          this->point[i] =
              this->witnessPoint[i];  // there should set some value to
                                      // witnessPoint even if the witness has bad
                                      // angle and deleted later.
      }
    } else
      this->witnessPoint[i] =
          this->param_lengthLower[v1][v2];  // probably for the dense case

    if (this->witnessPoint[i] < 0 ||
        std::isnan(this->witnessPoint[i])) {  // quick fix and for debug purpose
      this->witnessPoint[i] = this->param_lengthLower[v1][v2];
#ifdef VERBOSE
      cout << "ERRORRRRRRRRs" << endl;
#endif
    }

    if (this->witnessPoint[i] < this->param_lengthLower[v1][v2] &&
        this->witnessPoint[i] > this->param_lengthLower[v1][v2] - 0.1) {
      this->witnessPoint[i] = this->param_lengthLower[v1][v2];
    } else if (this->witnessPoint[i] > this->param_lengthUpper[v1][v2] &&
               this->witnessPoint[i] < this->param_lengthUpper[v1][v2] + 0.1) {
      this->witnessPoint[i] = this->param_lengthUpper[v1][v2];
    }

    if (this->witnessPoint[i] < this->param_lengthLower[v1][v2] ||
        this->witnessPoint[i] >
            this->param_lengthUpper[v1]
                                   [v2]) {  // quick fix and for debug purpose
#ifdef VERBOSE
      cout << "ERRORRRRRRRRs  witnessPoint is out of boundary "
           << this->witnessPoint[i] << " "
           << this->witnessPoint[i] - this->param_lengthLower[v1][v2] << " "
           << this->param_lengthUpper[v1][v2] - this->witnessPoint[i]
           << endl;  // one reason is; when a parameter becomes a contact, its
                     // value is set to be sum of radius. since it is forced to
                     // be exact contact, the range for the new parameters may
                     // not fit the value of witness orientation.
#endif
      this->witnessPoint[i] = this->param_lengthLower[v1][v2];
      this->point[i] = this->witnessPoint[i];
    }

    if (!contin) this->point[i] = this->witnessPoint[i];

    //-------added to test jacobian, it finds better starting point somehow when
    //it starts from the middle
    if (sett->Sampling.JacobianSampling) {
      this->witnessPoint[i] =
          (this->param_lengthLower[v1][v2] + this->param_lengthUpper[v1][v2]) /
          2;
      this->point[i] = this->witnessPoint[i];
    }
    //-------

    this->param_length[v1][v2] = this->point[i];
    this->param_length[v2][v1] = this->point[i];

  }  // point can be <= 6 dimensional. If it is 2 dimensional(when there is 2
     // parameter) then point[2,3,4,5] is null

  for (int i = 0; i < NO; i++)
    for (int j = i; j < NO; j++) {
      if (this->param_lengthLower[i][j] > this->param_lengthUpper[i][j]) {
        cout << "invalid description along parameter " << i << "-" << j
             << " min " << this->param_lengthLower[i][j] << " max "
             << this->param_lengthUpper[i][j] << endl;
        // return false;  //commented on 25oct11 since i arrange the range
        // according to param_length, it is possible that just current value of
        // param_length is blue
      }
    }

  this->gridDone = false;

  if (!contin) {
    for (size_t a = this->parameters.size(); a < 6; a++) {
      this->minOfMin[a] = 0;
      this->maxOfMax[a] = 0;
    }
    region->setSpaceVolume(this->minOfMin, this->maxOfMax);
  }

  // FOR JACOBIAN SAMPLING
  // set middle point
  for (int m = 0; m < 6; m++)
    for (size_t p = 0; p < this->parameters.size(); p++)
      this->mjpoint[m][p] = this->witnessPoint[p];

  return true;
}

void ConvexChart::setMinMaxParam(int v1, int v2) {
  this->edge_length[v2][v1] = this->edge_length[v1][v2];
  this->param_lengthUpper[v1][v2] = this->edge_length[v1][v2];
  this->param_lengthUpper[v2][v1] = this->edge_length[v1][v2];
  this->param_lengthLower[v1][v2] = this->edge_length[v1][v2];
  this->param_lengthLower[v2][v1] = this->edge_length[v1][v2];
  this->param_length[v1][v2] = this->edge_length[v1][v2];
  this->param_length[v2][v1] = this->edge_length[v1][v2];
}

void ConvexChart::setRangeByTriangleInequality(int v1, int v2) {
  int paramv1v2No = find_pair_in_vector(this->parameters, make_pair(v1, v2));

  if (this->edge_length[v1][v2] == -1 ||
      std::isnan(this->edge_length[v1][v2])) {
    double max = 100;
    for (int i = 0; i < NO; i++) {
      int paramv1iNo = find_pair_in_vector(
          this->parameters, make_pair(v1, i));  // pair = <v, i> is not ordered
      int paramv2iNo = find_pair_in_vector(this->parameters, make_pair(v2, i));
      if ((paramv1iNo >= 0 && paramv1iNo < paramv1v2No) ||
          (paramv2iNo >= 0 &&
           paramv2iNo <
               paramv1v2No))  // if v1-i is a parameter that is not set yet,
                              // then skip. order of the parameter is important.
        continue;  // todo instead of skipping why dont you use just the min or
                   // max value of it to decrease the range of this param
                   //       no, if you use it, it will lead you to wrong range,
                   //       since the parameter value will be set/fixed to
                   //       another value in next steps !

      double v1i = this->param_length[v1][i];
      if (v1i == -1) v1i = this->param_lengthUpper[v1][i];
      double iv2 = this->param_length[v2][i];
      if (iv2 == -1) iv2 = this->param_lengthUpper[v2][i];
      if (v1i != -1 && iv2 != -1 && max > (v1i + iv2)) {
        max = v1i + iv2;
      }
    }

    if (max != 100) {
      this->param_lengthUpper[v1][v2] = max;
      this->param_lengthUpper[v2][v1] = max;
    }

    double min = 0;
    for (int i = 0; i < NO; i++) {
      double localmin = 100;

      int paramv1iNo = find_pair_in_vector(this->parameters, make_pair(v1, i));
      int paramv2iNo = find_pair_in_vector(this->parameters, make_pair(v2, i));
      if ((paramv1iNo >= 0 && paramv1iNo < paramv1v2No) ||
          (paramv2iNo >= 0 && paramv2iNo < paramv1v2No))
        continue;

      double maxv1i = this->param_length[v1][i];
      double minv1i = this->param_length[v1][i];
      if (maxv1i == -1) maxv1i = this->param_lengthUpper[v1][i];
      if (minv1i == -1) minv1i = this->param_lengthLower[v1][i];

      double maxiv2 = this->param_length[v2][i];
      double miniv2 = this->param_length[v2][i];
      if (maxiv2 == -1) maxiv2 = this->param_lengthUpper[v2][i];
      if (miniv2 == -1) miniv2 = this->param_lengthLower[v2][i];

      //----------------
      if (minv1i > maxiv2 && minv1i != -1 && maxiv2 != -1)
        localmin = minv1i - maxiv2;  // finds the smallest difference of edges
                                     // not to restrict feasible intervals
      else if (miniv2 > maxv1i && miniv2 != -1 && maxv1i != -1)
        localmin = miniv2 - maxv1i;
      else
        localmin =
            0;  // otherwise min and max ranges are intermingled, hence at the
                // intersected places the difference of the edges is 0
      //----------------

      if (min < localmin && localmin != 100) min = localmin;
    }

    Atom* atomA = getAtom(v1);
    Atom* atomB = getAtom(v2);
    double collisionLowerBound = df->collisionLowerBound(atomA, atomB);

    //		if(sett->Collision::stericConstraint)  	// uncomment to allow
    //collison on parameters
    if (min < collisionLowerBound) min = collisionLowerBound;

    if (min != 0) {
      this->param_lengthLower[v1][v2] = min;
      this->param_lengthLower[v2][v1] = min;
    }

    // this case may happen if maxrange is less than sumofradii or by some
    // mysterious reason  // todo print to see it
    if (this->param_lengthLower[v1][v2] > this->param_lengthUpper[v1][v2]) {
      this->param_lengthLower[v1][v2] = this->param_lengthUpper[v1][v2];
      this->param_lengthLower[v2][v1] = this->param_lengthUpper[v1][v2];
    }
  } else
    setMinMaxParam(v1, v2);
}

int ConvexChart::find_pair_in_vector(vector<pair<int, int> > ivector,
                                     pair<int, int> ipair) {
  int index = -1;
  for (vector<pair<int, int> >::iterator iter = ivector.begin();
       iter != ivector.end(); iter++) {
    index++;
    if (((*iter).first == ipair.first && (*iter).second == ipair.second) ||
        ((*iter).first == ipair.second && (*iter).second == ipair.first))
      return index;
  }
  return -1;
}

vector<vector<int> > ConvexChart::getTetras() { return this->tetrahedra; }

Atom* ConvexChart::getAtom(int vertex) {
  if (vertex < NO / 2) {
    vertex = this->currentGraph->getA(vertex);
    return this->helA->getAtomAt(vertex);
  } else {
    vertex = this->currentGraph->getB(vertex - NO / 2);
    return this->helB->getAtomAt(vertex);
  }
}

void ConvexChart::setWitnessPoint(CayleyPoint* wp) {
  wp->getPoint(this->witnessPoint);
}

double ConvexChart::dynamicStepSize(int v1, int v2, int p) {
  Settings* sett = Settings::getInstance();
  double mid =
      (this->param_lengthUpper[v1][v2] + this->param_lengthLower[v1][v2]) / 2;
  double ratio = (mid - this->point[p]) / (this->param_lengthUpper[v1][v2] -
                                           this->param_lengthLower[v1][v2]);

  double linearStep = this->stepSize;
  if (sett->Sampling.dynamicStepSizeWithin == 1)
    linearStep = this->stepSize + 1.8 * this->stepSize * ratio;  // 4. / 3.
  else if (sett->Sampling.dynamicStepSizeWithin == 2)
    linearStep = this->stepSize - 1.8 * this->stepSize * ratio;  // 4. / 3.

  if (this->param_lengthUpper[v1][v2] == this->param_lengthLower[v1][v2])
    linearStep = this->stepSize;

  return linearStep;
}

Orientation* ConvexChart::computeRealization(
    int flipNo, bool& fail, const vector<vector<int> >& flipScheme) {
  Settings* sett = Settings::getInstance();
  fail = false;

  bool realizable;
  double *fromA[3], *fromB[3];
  double *toA[3], *toB[3];
  double positions[NO][3];
  double tempParamLength[NO][NO];

  for (int i = 0; i < NO; i++) {
    for (int j = 0; j < NO; j++) {
      tempParamLength[i][j] = param_length[i][j];
    }
  }

  vector<vector<int> > tempTetrahedra;

  fromA[0] = helA->getAtomAt(verticesA[0])->getLocation();
  fromA[1] = helA->getAtomAt(verticesA[1])->getLocation();
  fromA[2] = helA->getAtomAt(verticesA[2])->getLocation();

  fromB[0] = helB->getAtomAt(verticesB[0])->getLocation();
  fromB[1] = helB->getAtomAt(verticesB[1])->getLocation();
  fromB[2] = helB->getAtomAt(verticesB[2])->getLocation();
  realizable = true;

  for (int i = 0; i < NO; i++) {
    positions[i][0] = -111;
    positions[i][1] = -111;
    positions[i][2] = -111;
  }

  vector<bool> mirrorlist;
  mirrorlist.push_back(flipNo % 2 == 1);
  mirrorlist.push_back((flipNo / 2) % 2 == 1);
  mirrorlist.push_back((flipNo / 4) % 2 == 1);

  for (int i = 0; i < 3; i++) {
    toA[i] = new double[3];
    toB[i] = new double[3];
  }

  if (partial3tree) {
    tempTetrahedra = getTetras();
  } else {
    tempTetrahedra = flipScheme;
  }

  size_t mirrorIndex = 1;
  if (setBaseTetra(mirrorlist[0], realizable, tempTetrahedra, positions,
                   tempParamLength))  // if indices are in the same helix , then
                                      // no need for mirror
    mirrorIndex = 0;
  if (!realizable) {
    fail = true;
    for (int i = 0; i < 3; i++) {
      delete[] toA[i];
      delete[] toB[i];
    }
    return NULL;
  }

  // builds new locations onto the base tetrahedron.
  size_t tetraIndex = 1;
  do {
    if (!locateVertex(
            tetraIndex, mirrorlist[mirrorIndex], realizable, tempTetrahedra,
            positions,
            tempParamLength))  // if locateVertex is true, then indices are in
                               // the same helix, then no need for mirror and
                               // volume is positive
    {
      if (!realizable) {
        fail = true;
        for (int i = 0; i < 3; i++) {
          delete[] toA[i];
          delete[] toB[i];
        }
        return NULL;
      }
      computeLengthOfTheEdgesConnectedToVertex(tetrahedra[tetraIndex][0],
                                               positions, tempParamLength);
      mirrorIndex++;
    }
    tetraIndex++;
  } while (tetraIndex < tempTetrahedra.size());
  // now it should be able to fill all the values of vertices, else it means it
  // needs maple

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      toA[i][j] = positions[i][j];
      toB[i][j] = positions[i + NO / 2][j];
      if (toA[i][j] == -111 || toB[i][j] == -111 || std::isnan(toA[i][j]) ||
          std::isnan(toB[i][j]))
        cout << "eroorrrr toA[i][j]== -111 || toB[i][j]== -111" << endl;
    }
  }

  if (isDistorted(toA, fromA)) {
    realizable = false;
    fail = true;
    for (int i = 0; i < 3; i++) {
      delete[] toA[i];
      delete[] toB[i];
    }
    return NULL;
  }

  // TRANSLATE HELIX B ACCORDING TO HELIX A
  for (int j = 0; j < 3; j++) {  // tranlates toB[j]
    double pot[3];
    Utils::matApp(toB[j], toA[0], toA[1], toA[2], fromA[0], fromA[1], fromA[2],
                  pot);

    for (int i = 0; i < 3; i++) {
      toB[j][i] = pot[i];
    }
  }

  // FIX HELIX A
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      toA[i][j] = fromA[i][j];
    }
  }

  // end original if
  //}

  Eigen::Vector3d TB, eaB;

  if (sett->Sampling.JacobianSampling) compute_TB_EAB(toB, fromB, TB, eaB);

  Orientation* ori_output = new Orientation(fromB, toB);
  ori_output->setFlipNum(flipNo);
  if (sett->Sampling.JacobianSampling) {
    ori_output->TB = TB;
    ori_output->eaB = eaB;
  }
  for (int i = 0; i < 3; i++) {
    delete[] toA[i];
    delete[] toB[i];
  }
  return ori_output;
}

void ConvexChart::compute_TB_EAB(double* toB[3], double* fromB[3],
                                 Eigen::Vector3d& TB, Eigen::Vector3d& eaB) {
  Vector3d p1(fromB[0][0], fromB[0][1], fromB[0][2]);
  Vector3d p2(fromB[1][0], fromB[1][1], fromB[1][2]);
  Vector3d p3(fromB[2][0], fromB[2][1], fromB[2][2]);

  Vector3d P1(toB[0][0], toB[0][1], toB[0][2]);
  Vector3d P2(toB[1][0], toB[1][1], toB[1][2]);
  Vector3d P3(toB[2][0], toB[2][1], toB[2][2]);

  Vector3d v1 = p2 - p1;
  Vector3d v2 = p3 - p1;
  Vector3d v3 = v1.cross(v2);

  Vector3d V1 = P2 - P1;
  Vector3d V2 = P3 - P1;
  Vector3d V3 = V1.cross(V2);

  Matrix3d m, M, R;
  m << v1(0), v2(0), v3(0), v1(1), v2(1), v3(1), v1(2), v2(2), v3(2);
  M << V1(0), V2(0), V3(0), V1(1), V2(1), V3(1), V1(2), V2(2), V3(2);

  R = M * m.inverse();       // rotation matrix
  Vector3d t = P1 - R * p1;  // translation

  // compute mean
  vector<Atom*> xfHelB = helB->getAtoms();
  Vector3d mean(0, 0, 0);
  for (size_t iter = 0; iter < xfHelB.size(); iter++) {
    double* l = xfHelB[iter]->getLocation();
    Vector3d p(l[0], l[1], l[2]);
    mean += R * p;
  }
  mean = mean / xfHelB.size();

  Vector3d TB_ = t + mean;

  //  Quaterniond q(R);
  //	Matrix3d RB = quaterniontoRotation( q.w(), q.x(),  q.y(), q.z() );
  //	Vector3d eaB_= RB.eulerAngles(2, 0, 2); //Cardan angle (ZYZ) rotation.
  //	eaB_ = eaB_ * 180 / PI;
  //	double cos_eaB1 = cos(-eaB_[1]*PI/180);

  Vector3d eaB_ = Utils::RotMatToEuler(R);
  eaB_[0] = eaB_[0] * 180 / PI;
  eaB_[2] = eaB_[2] * 180 / PI;
  double cos_eaB1 = eaB_[1];
  //	eaB_[1] = acos(eaB_[1]) * 180 / PI;

  // eaB_[1] = abs( eaB_[1] ); //FOR TEST REASON SINCE There are, however, other
  // sets of Euler angles which give the same rotation matrix
  //	int sign = Utils::sign(cos_eaB1);  //R(a,b,c) = R(-a, b+PI, -c) = R( -a,
  //b-PI, -c) and since  cos(b+PI)=cos(b-PI)= -cos(b) so when I invert the sign
  //of cos(eab1), we need to also invert the sign of eab0 and eab2 hence if you
  // take abs value of eab1, you should invert eab0, eab2 as well.

  //	if(sign == -1)
  //	{
  //		eaB_[1] = eaB_[1] - 180;
  //		eaB_[0] = sign * eaB_[0];
  //		eaB_[2] = sign * eaB_[2];
  //		cos_eaB1 = abs(cos_eaB1);
  //	}
  // also i am assuming eaB_= RB.eulerAngles(2, 0, 2); always gives eab0 and
  // eab2 in between [-180, 180] by observation

  TB = TB_;
  eaB = eaB_;
}

// for better result, additionally we can check the transformation from fromB to
// toB as well
bool ConvexChart::isDistorted(double* toA[3], double* fromA[3]) {
  // creates 2 points p1 and p2 where the distance between them is exactly 1
  double* p1 = new double[3];
  p1[0] = 0;
  p1[1] = 0;
  p1[2] = 0;
  double* p2 = new double[3];
  p2[0] = 0;
  p2[1] = 0;
  p2[2] = 1;

  // compute pt1 and pt2, the transformed location of p1 and p2 using the same
  // transformation from fromA to toA
  double pt1[3];
  double pt2[3];
  Utils::matApp(p1, toA[0], toA[1], toA[2], fromA[0], fromA[1], fromA[2], pt1);
  Utils::matApp(p2, toA[0], toA[1], toA[2], fromA[0], fromA[1], fromA[2], pt2);

  // compute the distance between pt1 and pt2
  double dist = Utils::dist(pt1, pt2);
  delete[] p1;
  delete[] p2;

  // the ce should approximate to 1, otherwise there is a distortion
  if (dist < .9 || dist > 1.1) {
    return true;
  }
  return false;
}

bool ConvexChart::setBaseTetra(bool mirror, bool& realizable,
                               vector<vector<int> >& tempTetrahedra,
                               double positions[NO][3],
                               double tempParamLength[NO][NO]) {
  vector<int> inds = tempTetrahedra.front();
  if (inds[0] < NO / 2 && inds[1] < NO / 2 && inds[2] < NO / 2 &&
      inds[3] < NO / 2) {  // inside of same helix
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 3; j++)
        positions[inds[i]][j] =
            helA->getAtomAt(verticesA[inds[i]])->getLocation()[j];
    }
    return true;
  } else if (inds[0] >= NO / 2 && inds[1] >= NO / 2 && inds[2] >= NO / 2 &&
             inds[3] >= NO / 2) {  // inside of same helix
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 3; j++)
        positions[inds[i]][j] =
            helB->getAtomAt(verticesB[inds[i] - NO / 2])->getLocation()[j];
    }
    return true;
  }
#ifdef VERBOSE
  // cout<<  "base " << inds[0] << " " << inds[1] << " " << inds[2]<< " " <<
  // inds[3] << endl;
#endif

  double posi[4][3];
  locateTetrahedron(0, realizable, posi, mirror, tempParamLength);
  for (int i = 0; i < 4; i++) {
    positions[inds[i]][0] = posi[i][0];  // x coordinate
    positions[inds[i]][1] = posi[i][1];  // y coordinate
    positions[inds[i]][2] = posi[i][2];  // z coordinate
  }

  return false;
}

void ConvexChart::locateTetrahedron(int tetrahedron_index, bool& realizable,
                                    double posi[][3], bool mirror,
                                    double tempParamLength[NO][NO]) {
  double position[4][3];
  double edgeLengths[6];
  vector<int> inds = tetrahedra[tetrahedron_index];

  edgeLengths[0] = tempParamLength[inds[0]][inds[1]];
  edgeLengths[1] = tempParamLength[inds[2]][inds[3]];
  edgeLengths[2] = tempParamLength[inds[0]][inds[2]];
  edgeLengths[3] = tempParamLength[inds[1]][inds[3]];
  edgeLengths[4] = tempParamLength[inds[0]][inds[3]];
  edgeLengths[5] = tempParamLength[inds[1]][inds[2]];

  double vc = Utils::tetrahedralVolumeUsingCayleyMengerDeterminant(edgeLengths);
  tetra_volume_[tetrahedron_index] = vc;
  for (int i=0; i<6; i++) {
    tetra_edges_[tetrahedron_index][i] = edgeLengths[i];
  }

  vc += 0.0000001;  // if vs= -..e^-...

#ifdef VERBOSE
  // cout << "vc2 " << vc << endl;
#endif

  if (vc < -0.0000002) {
    realizable = false;
    is_tetra_volume_zero_[tetrahedron_index] = false;
    return;
  } else if (vc < 0.0000002) {
    is_tetra_volume_zero_[tetrahedron_index] = true;
  } else {
    is_tetra_volume_zero_[tetrahedron_index] = false;
  }

  bool foundNan = false;
  Utils::lenToTetra(edgeLengths, position[0], position[1], position[2],
                    position[3], mirror);
  for (int i = 0; i < 4; i++) {
    posi[i][0] = position[i][0];
    posi[i][1] = position[i][1];
    posi[i][2] = position[i][2];
    if (std::isnan(posi[i][0]) || std::isnan(posi[i][1]) ||
        std::isnan(posi[i][2]))
      foundNan = true;
  }

  bool disturbed = false;
  for (int i = 0; i < 4; i++) {
    for (int j = i + 1; j < 4; j++) {
      if (abs(tempParamLength[inds[i]][inds[j]] -
              Utils::dist(posi[i], posi[j])) > .6) {
        disturbed = true;
      }
    }
  }

  // if you enter here when volume not exactly zero, you may disturb your
  // lengths ! so enter here if the lengths are already disturbed.
  if ((vc < 15 && vc >= 0 && disturbed) || foundNan) {
    posi[0][0] = 0;
    posi[0][1] = 0;
    posi[0][2] = 0;
    posi[1][0] = tempParamLength[inds[0]][inds[1]];
    posi[1][1] = 0;
    posi[1][2] = 0;

    double ac = tempParamLength[inds[0]][inds[2]],
           ab = tempParamLength[inds[0]][inds[1]],
           bc = tempParamLength[inds[1]][inds[2]];
    double cosCAB = (ac * ac + ab * ab - bc * bc) / (2 * ac * ab);
    double sinCAB = sqrt(abs(1 - cosCAB * cosCAB));
    posi[2][0] = tempParamLength[inds[0]][inds[2]] * cosCAB;
    posi[2][1] = tempParamLength[inds[0]][inds[2]] * sinCAB;
    posi[2][2] = 0;

    ac = tempParamLength[inds[0]][inds[3]];
    ab = tempParamLength[inds[0]][inds[1]];
    bc = tempParamLength[inds[1]][inds[3]];
    cosCAB = (ac * ac + ab * ab - bc * bc) / (2 * ac * ab);
    sinCAB = sqrt(abs(1 - cosCAB * cosCAB));
    posi[3][0] = tempParamLength[inds[0]][inds[3]] * cosCAB;
    posi[3][1] = tempParamLength[inds[0]][inds[3]] * sinCAB;
    posi[3][2] = 0;

    double nposi[2];
    nposi[0] = posi[3][0];
    nposi[1] = -1 * posi[3][1];
    double cd1 = sqrt((posi[2][0] - posi[3][0]) * (posi[2][0] - posi[3][0]) +
                      (posi[2][1] - posi[3][1]) * (posi[2][1] - posi[3][1]));
    double cd2 = sqrt((posi[2][0] - nposi[0]) * (posi[2][0] - nposi[0]) +
                      (posi[2][1] - nposi[1]) * (posi[2][1] - nposi[1]));
    double cd = tempParamLength[inds[2]][inds[3]];

    if (abs(cd - cd1) > abs(cd - cd2)) posi[3][0] = nposi[0];
    posi[3][1] = nposi[1];
  }
}

void ConvexChart::computeLengthOfTheEdgesConnectedToVertex(
    int vertex, double positions[NO][3], double tempParamLength[NO][NO]) {
  for (int i = 0; i < NO; i++) {
    if (positions[i][0] != -111 &&
        tempParamLength[i][vertex] ==
            -1) {  // if ith vertex is located before and if the distance
                   // i-vertex is not defined before
      tempParamLength[i][vertex] = tempParamLength[vertex][i] =
          Utils::dist(positions[i], positions[vertex]);
#ifdef VERBOSE
      //	    cout << "tempParamLength["<<i<<"]["<<vertex<<"] " <<
      //tempParamLength[i][vertex] <<endl;
#endif
    }
  }
}

void ConvexChart::printPositionsAndConnections(const vector<int>& ind,
                                               double positions[NO][3],
                                               double tempParamLength[NO][NO]) {
  cout << "indices " << ind[0] << " " << ind[1] << " " << ind[2] << " "
       << ind[3] << endl;

  for (int i = 0; i < 4; i++) {
    cout << ind[i] << ": " << positions[ind[i]][0] << " "
         << positions[ind[i]][1] << " " << positions[ind[i]][2] << endl;
    for (int j = i + 1; j < 4; j++) {
      cout << ind[i] << "-" << ind[j] << ": " << tempParamLength[ind[i]][ind[j]]
           << " " << Utils::dist(positions[ind[i]], positions[ind[j]]) << endl;
    }
  }

  cout << endl;
}

bool ConvexChart::locateVertex(int tetrahedron_index, bool mirror,
                               bool& realizable,
                               const vector<vector<int> >& tempTetrahedra,
                               double positions[NO][3],
                               double tempParamLength[NO][NO]) {
  double posi[4][3];

  vector<int> ind = tempTetrahedra[tetrahedron_index];

#ifdef VERBOSE
  // printPositionsAndConnections(ind, positions);
#endif

  if (ind[0] < NO / 2 && ind[1] < NO / 2 && ind[2] < NO / 2 &&
      ind[3] < NO / 2) {  // inside of same helix i.e. helix A
    double* from[4];
    for (int i = 0; i < 4; i++) {
      from[i] = helA->getAtomAt(verticesA[ind[i]])->getLocation();
    }
    double pot[3];
    Utils::matApp(from[0], from[1], from[2], from[3], positions[ind[1]],
                  positions[ind[2]], positions[ind[3]], pot);

    for (int i = 0; i < 3; i++) {
      positions[ind[0]][i] = pot[i];
    }

    return true;
  } else if (ind[0] >= NO / 2 && ind[1] >= NO / 2 && ind[2] >= NO / 2 &&
             ind[3] >= NO / 2) {  // inside of same helix i.e. helix B
    double* from[4];
    for (int i = 0; i < 4; i++) {
      from[i] = helB->getAtomAt(verticesB[ind[i] - NO / 2])->getLocation();
    }
    double pot[3];
    Utils::matApp(from[0], from[1], from[2], from[3], positions[ind[1]],
                  positions[ind[2]], positions[ind[3]], pot);

    for (int i = 0; i < 3; i++) {
      positions[ind[0]][i] = pot[i];
    }

    return true;
  }

  locateTetrahedron(tetrahedron_index, realizable, posi, mirror,
                    tempParamLength);
  if (!realizable) return false;

  //	debug = false;
  double pot[3];
  Utils::matApp(posi[0], posi[1], posi[2], posi[3], positions[ind[1]],
                positions[ind[2]], positions[ind[3]], pot);

  for (int i = 0; i < 3; i++) {
    positions[ind[0]][i] = pot[i];
#ifdef VERBOSE
    //	cout << positions[ ind[0] ][i] << endl;
#endif
  }

#ifdef VERBOSE
  // cout << ind[0] << "-" << ind[1] << "   " << ind[0] << "-" << ind[2] << " "
  // << ind[0] << "-" << ind[3] << "   " << endl; cout << "fact " <<
  // tempParamLength[ind[0]][ind[1]] << " "<< tempParamLength[ind[0]][ind[2]] <<
  // " "<< tempParamLength[ind[0]][ind[3]] << endl; cout << "before "<<
  // Utils::dist(posi[0], posi[1]) << " "<< Utils::dist(posi[0], posi[2]) << "
  // "<< Utils::dist(posi[0], posi[3]) << endl; cout << "later "<<
  // Utils::dist(positions[ ind[0] ],positions[ ind[1] ]) << " "<<
  // Utils::dist(positions[ ind[0] ], positions[ ind[2] ]) << " "<<
  // Utils::dist(positions[ ind[0] ], positions[ ind[3] ]) << endl;

#endif

  bool foundNan = false;  // if volume is close to 0, even if locateTetrahedron
                          // gives correct numbers after matApp, they become nan.
                          // matApp doesnot work for volume 0 case. It cannot
                          // compute translation matrix for this case!
  for (int i = 0; i < 4; i++) {
    if (std::isnan(positions[ind[i]][0]) || std::isnan(positions[ind[i]][1]) ||
        std::isnan(positions[ind[i]][2]))
      foundNan = true;
  }

  bool disturbed = false;
  for (int i = 0; i < 4; i++) {
    for (int j = i + 1; j < 4; j++) {
      if (abs(Utils::dist(posi[i], posi[j]) -
              Utils::dist(positions[ind[i]], positions[ind[j]])) > .6)
        disturbed = true;
    }
  }

  if (disturbed || foundNan) {
#ifdef VERBOSE
    /*
            for(int i=1; i<4; i++) {
                    cout << ind[i] << ": " << positions[ind[i]][0] << " " <<
       positions[ind[i]][1] << " " << positions[ind[i]][2] << endl; for(int
       j=i+1; j<4; j++) { cout << ind[i] << "-" << ind[j] << ": " <<
       tempParamLength[ind[i]][ind[j]] << " " <<  Utils::dist(positions[ ind[i]
       ],positions[ ind[j] ]) << endl;
        }
            }*/
#endif

    double* pot = Utils::find4thPoint_inTheSamePlane(
        positions[ind[1]], positions[ind[2]], positions[ind[3]],
        tempParamLength[ind[0]][ind[1]], tempParamLength[ind[0]][ind[2]],
        tempParamLength[ind[0]][ind[3]]);

    for (int i = 0; i < 3; i++) {
      positions[ind[0]][i] = pot[i];
    }

#ifdef VERBOSE
    //	cout << ind[0] << ": " << positions[ind[0]][0] << " " <<
    //positions[ind[0]][1] << " " << positions[ind[0]][2] << endl;
#endif
    delete[] pot;

    if (std::isnan(positions[ind[0]][0]) || std::isnan(positions[ind[0]][1]) ||
        std::isnan(
            positions[ind[0]][2]))  // if volume is close to 0, even if
                                    // locateTetrahedron gives correct numbers
                                    // after matApp, they become nan. matApp
                                    // doesnot work for volume 0 case. It cannot
                                    // compute translation matrix for this case!
      realizable = false;
#ifdef VERBOSE /*                                                              \
                cout << ind[0] << "-" << ind[1] << "   " << ind[0] << "-" <<   \
                ind[2] << "   " << ind[0] << "-" << ind[3] << "   " << endl;   \
                cout << "fact " << tempParamLength[ind[0]][ind[1]] << " "<<    \
                tempParamLength[ind[0]][ind[2]] << " "<<                       \
                tempParamLength[ind[0]][ind[3]] << endl; cout << "later "<<                                                            \
                Utils::dist(positions[ ind[0] ],positions[ ind[1] ]) << " "<<  \
                Utils::dist(positions[ ind[0] ], positions[ ind[2] ]) << " "<< \
                Utils::dist(positions[ ind[0] ], positions[ ind[3] ]) <<       \
                endl;*/
#endif
    if (abs(tempParamLength[ind[0]][ind[1]] -
            Utils::dist(positions[ind[0]], positions[ind[1]])) > .6)
      realizable = false;
    if (abs(tempParamLength[ind[0]][ind[2]] -
            Utils::dist(positions[ind[0]], positions[ind[2]])) > .6)
      realizable = false;
    if (abs(tempParamLength[ind[0]][ind[3]] -
            Utils::dist(positions[ind[0]], positions[ind[3]])) > .6)
      realizable = false;
  }

  //	debug = false;
  return false;
}

void ConvexChart::stepGrid() {
  bool dens = false;
  if (dense) {
    for (size_t i = 1; i < this->parameters.size(); i++) {
      double x =
          (this->point[i] - this->witnessPoint[i]) / (2 * this->stepSize);
      int y = ceil(x);
      x = x - y;
      if (abs(x) > 0.0001 &&
          abs(x) <
              0.9999)  //  abs(x - int(x))>0.0001  //if point is in the middle
        dens = true;
    }
  }

  double temp, temp2;
  bool carry = true;
  int p = 0;
  int v1, v2;
  for (size_t i = 0; i < this->parameters.size(); i++) {
    if (carry) {
      v1 = this->parameters[i].first;
      v2 = this->parameters[i].second;

      double tempStepSize = this->stepSize;
      this->stepSize = dynamicStepSize(v1, v2, p);

      double tmpoint = this->witnessPoint[p];
      if (this->witnessPoint[p] <
          this->param_lengthLower[v1]
                                 [v2])  // min and max connection is changing
                                        // since i compute the range continuously
        tmpoint = this->param_lengthLower[v1][v2];
      else if (this->witnessPoint[p] > this->param_lengthUpper[v1][v2])
        tmpoint = this->param_lengthUpper[v1][v2];

      if (this->point[p] >= tmpoint)
        this->point[p] += this->stepSize;

      else
        this->point[p] -= this->stepSize;

      if (dense && p == 0 &&
          !dens)  //  ! if at least one of the other parameters are in the
                  //  middle of step interval
      {
        temp = tmpoint + this->stepSize;
        temp2 = tmpoint - this->stepSize;
        if (this->point[0] > temp)  // != // to start from offset point
          this->point[p] += this->stepSize;
        else if (this->point[0] < temp2)  // !=
          this->point[p] -= this->stepSize;
      }

      temp = this->param_lengthUpper[v1][v2] + this->stepSize;
      if (this->point[p] > this->param_lengthUpper[v1][v2] &&
          this->point[p] < temp) {
        this->point[p] = this->param_lengthUpper[v1][v2];
      } else if (this->point[p] > this->param_lengthUpper[v1][v2]) {
        this->point[p] = tmpoint - this->stepSize;
        if (this->point[p] >
            this->param_lengthUpper[v1][v2])  // maxconnection is changing since
                                              // i compute the range continuously
          this->point[p] = this->param_lengthUpper[v1][v2];
      }
      carry = false;
      this->param_length[v1][v2] = this->point[p];
      this->param_length[v2][v1] = this->point[p];

      temp = this->param_lengthLower[v1][v2] - this->stepSize;
      if (this->point[p] < this->param_lengthLower[v1][v2] &&
          this->point[p] > temp) {
        this->point[p] = this->param_lengthLower[v1][v2];
        this->param_length[v1][v2] = this->point[p];
        this->param_length[v2][v1] = this->point[p];
        carry = false;
      } else if (this->point[p] < this->param_lengthLower[v1][v2]) {
        carry = true;
        this->point[p] = tmpoint;

        //				if(tmpoint < this->edge_lengthLower[v1][v2])
        ////minconnection is changing since i compute the range continuously
        //					this->point[p] =
        //this->edge_lengthLower[v1][v2]; 				if(tmpoint >
        //this->edge_lengthUpper[v1][v2]) //maxconnection is changing since i
        //compute the range continuously 					this->point[p] =
        //this->edge_lengthUpper[v1][v2];

        this->param_length[v1][v2] = this->point[p];
        this->param_length[v2][v1] = this->point[p];
        p++;
      }

      this->stepSize = tempStepSize;
    }
  }

  for (int x = p; x >= 0; x--) {
    // should update min max range for parameters from 0th till p'th  not just
    // p'th parameter because if p'th parameters value is changed then that means
    // every previous parameters value is also changed by setting it to mid
    // value.

    // todo then why do you have 2 for loops and updateList, the first loop
    // would be sufficient?!! not actually! wouldnot be sufficient. updating xth
    // parameter may not require the range computation for x-1th param, so if you
    // do not check dependency you may do extra work, for instance  a->b and
    // a->c->d case changing b doesnot require update on c however if there is a
    // circular type of dependency, for example 3 non-edges in one tetrahedra,
    // like a->b->c and a->c then changing 'a' requires updates on the range of b
    // and c, then changing the param value of b requires updates on c. As a
    // result the range of c will be computed twice! bad.
    if (x < this->parameters.size()) {
      for (int i = 0; i < updateList[x].size(); i++) {
        int paramNo = updateList[x][i];
        if (boundaryComputationWay[paramNo] !=
            -2)  // to check if it is not contact
        {
          v1 = this->parameters[paramNo].first;
          v2 = this->parameters[paramNo].second;

          computeRange(v1, v2, paramNo);
        }
      }
    }
  }

  for (int x = p - 1; x >= 0;
       x--) {  // should set it to midpoint after min max range is recomputed
    if (boundaryComputationWay[x] != -2)  // to check if it is not contact
    {
      v1 = this->parameters[x].first;
      v2 = this->parameters[x].second;

      this->point[x] =
          this->witnessPoint[x];  // to make it always start from real mid point
                                  // not tmpoint one. since tmpoint is rounded
                                  // version of the moment when witnessPoint
                                  // hits the boundary.
      if (this->point[x] < this->param_lengthLower[v1][v2]) {
        this->point[x] = this->param_lengthLower[v1][v2];
        this->param_length[v1][v2] = this->point[x];
        this->param_length[v2][v1] = this->point[x];
      } else if (this->point[x] > this->param_lengthUpper[v1][v2]) {
        this->point[x] = this->param_lengthUpper[v1][v2];
        this->param_length[v1][v2] = this->point[x];
        this->param_length[v2][v1] = this->point[x];
      }
    }
  }
  // todo: i think those 2 for loops should merged, because when xth param value
  // changed to exact mid value, the boundary of the parameters less than x, will
  // change again.

  this->gridDone = carry;
}

void ConvexChart::updateBoundariesDependent(int x) {
  for (int i = 0; i < updateList[x].size(); i++) {
    int paramNo = updateList[x][i];
    if (boundaryComputationWay[paramNo] != -2)  // to check if it is not contact
    {
      int v1 = this->parameters[paramNo].first;
      int v2 = this->parameters[paramNo].second;

      computeRange(v1, v2, paramNo);
    }
  }
}

void ConvexChart::updateBoundariesSpecific(int paramNo) {
  int v1 = this->parameters[paramNo].first;
  int v2 = this->parameters[paramNo].second;

  double curr = param_length[v1][v2];

  computeRange(v1, v2, paramNo);

  param_length[v1][v2] =
      curr;  // since setRangeByTetrahedralInequality changes params value
  param_length[v2][v1] = param_length[v1][v2];
}

void ConvexChart::computeRange(int v1, int v2, int paramNo) {
  if (boundaryComputationWay[paramNo] == -1)
    setRangeByTriangleInequality(v1, v2);
  else
    setRangeByTetrahedralInequality(
        v1, v2,
        boundaryComputationWay[paramNo]);  // this method not only sets min, max
                                           // but also param ! check if that is
                                           // what u want !!
}

void ConvexChart::updateBoundaries() {
  Settings* sett = Settings::getInstance();
  for (int i = this->parameters.size() - 1; i >= 0;
       i--) {  // to start from reverse if you put contacts at the end
    int v1 = this->parameters[i].first;
    int v2 = this->parameters[i].second;
    int isContact = find_pair_in_vector(this->contacts, this->parameters[i]);

    if (isContact >= 0 && sett->Sampling.short_range_sampling) {
      this->param_lengthUpper[v1][v2] =
          df->bondingUpperBound(getAtom(v1), getAtom(v2));
      this->param_lengthLower[v1][v2] =
          df->bondingLowerBound(getAtom(v1), getAtom(v2));
    } else {
      double curr = param_length[v1][v2];

      computeRange(v1, v2, i);

      param_length[v1][v2] =
          curr;  // since setRangeByTetrahedralInequality changes params value
      param_length[v2][v1] = param_length[v1][v2];
    }
  }
}

void ConvexChart::setInitialPoint(vector<double> p) {
  for (size_t i = 0; i < this->parameters.size(); i++) {
    int v1 = this->parameters[i].first;
    int v2 = this->parameters[i].second;
    this->point[i] = p[i];
    this->param_length[v1][v2] = this->point[i];
    this->param_length[v2][v1] = this->point[i];
  }
  bin = 1;
}

void ConvexChart::setDir() { dir = -1; }

bool ConvexChart::stepAround() {
  stepSize_of_stepGridContact = this->stepSize;

  if (dir == 2 * this->parameters.size() - 1)
    return true;  // done
  else {
    dir++;

    int v1 = this->parameters[dir / 2].first;
    int v2 = this->parameters[dir / 2].second;

    if (dir % 2 == 0)
      this->point[dir / 2] += this->stepSize;  // forward direction
    else
      this->point[dir / 2] -= this->stepSize;  // backward direction
    // allow this->point to be less than edge_lengthLower since the purpose of
    // this method is to search for a colliding config to start binary search in
    // between.

    if (this->point[dir / 2] > this->param_lengthUpper[v1][v2]) {
      stepSize_of_stepGridContact =
          this->stepSize -
          (this->point[dir / 2] - this->param_lengthUpper[v1][v2]);
      this->point[dir / 2] = this->param_lengthUpper[v1][v2];
    }

    this->param_length[v1][v2] = this->point[dir / 2];
    this->param_length[v2][v1] = this->point[dir / 2];

    return false;
  }
}

void ConvexChart::stepGridBinary(bool valid) {
  int v1 = this->parameters[dir / 2].first;
  int v2 = this->parameters[dir / 2].second;

  int times = -1;  // walk through colliding point.
  if (!valid)      // collision
    times = 1;     // walk through valid point

  if (dir % 2 == 0)
    this->point[dir / 2] -= times * stepSize_of_stepGridContact /
                            pow(2., bin);  // forward direction
  else
    this->point[dir / 2] += times * stepSize_of_stepGridContact /
                            pow(2., bin);  // backward direction

  bin++;  // to half the step size one more time during binary search

  if (this->point[dir / 2] > this->param_lengthUpper[v1][v2])
    this->point[dir / 2] = this->param_lengthUpper[v1][v2];

  if (this->point[dir / 2] < this->param_lengthLower[v1][v2])  ///
    this->point[dir / 2] = this->param_lengthLower[v1][v2];

  this->param_length[v1][v2] = this->point[dir / 2];
  this->param_length[v2][v1] = this->point[dir / 2];
}

bool ConvexChart::doneGrid() { return this->gridDone; }

vector<double> ConvexChart::getPoint() {
  vector<double> out;
  for (size_t i = 0; i < this->parameters.size(); i++) out.push_back(point[i]);
  return out;
}

vector<double> ConvexChart::getParamPoint() {
  vector<double> out;
  for (size_t p = 0; p < this->parameters.size(); p++) {
    int v1 = this->parameters[p].first;
    int v2 = this->parameters[p].second;
    out.push_back(this->param_length[v1][v2]);
  }
  return out;
}

void ConvexChart::setRangeByTetrahedralInequality(int v1, int v2, int tetraNo) {
  setRangeByTriangleInequality(v1, v2);

  int inds[4];
  int no = 0;
  for (int m = 0; m < 4; m++)
    if (this->tetrahedra[tetraNo][m] != v1 &&
        this->tetrahedra[tetraNo][m] != v2) {
      inds[no] = this->tetrahedra[tetraNo][m];
      no++;
    }

  inds[2] = v1;
  inds[3] = v2;

  // set the range of the parameter( inds[2]-inds[3] ) according to tetrahedra
  // equality

  if (param_length[inds[0]][inds[1]] == -1 ||
      param_length[inds[0]][inds[2]] == -1 ||
      param_length[inds[1]][inds[2]] == -1 ||
      param_length[inds[0]][inds[3]] == -1 ||
      param_length[inds[1]][inds[3]] == -1)
    return;  // in order to compute tetrahedral range all those edges have to
             // have values. If the value of the edge will be computed in
             // realization class by known positions of vertices, they may not
             // have a value yet. In this case, we can not compute the range.

  double posi[4][3];
  posi[0][0] = 0;
  posi[0][1] = 0;
  posi[0][2] = 0;
  posi[1][0] = param_length[inds[0]][inds[1]];
  posi[1][1] = 0;
  posi[1][2] = 0;

  double ac = param_length[inds[0]][inds[2]],
         ab = param_length[inds[0]][inds[1]],
         bc = param_length[inds[1]][inds[2]];
  double cosCAB = (ac * ac + ab * ab - bc * bc) / (2 * ac * ab);
  double sinCAB = sqrt(abs(1 - cosCAB * cosCAB));
  posi[2][0] = param_length[inds[0]][inds[2]] * cosCAB;
  posi[2][1] = param_length[inds[0]][inds[2]] * sinCAB;
  posi[2][2] = 0;

  ac = param_length[inds[0]][inds[3]];
  ab = param_length[inds[0]][inds[1]];
  bc = param_length[inds[1]][inds[3]];
  cosCAB = (ac * ac + ab * ab - bc * bc) / (2 * ac * ab);
  sinCAB = sqrt(abs(1 - cosCAB * cosCAB));
  posi[3][0] = param_length[inds[0]][inds[3]] * cosCAB;
  posi[3][1] = param_length[inds[0]][inds[3]] * sinCAB;
  posi[3][2] = 0;

  double nposi[2];
  nposi[0] = posi[3][0];
  nposi[1] = -1 * posi[3][1];
  double minrange = sqrt((posi[2][0] - posi[3][0]) * (posi[2][0] - posi[3][0]) +
                         (posi[2][1] - posi[3][1]) * (posi[2][1] - posi[3][1]));
  double maxrange = sqrt((posi[2][0] - nposi[0]) * (posi[2][0] - nposi[0]) +
                         (posi[2][1] - nposi[1]) * (posi[2][1] - nposi[1]));

  if (minrange > maxrange) {
    double temp = minrange;
    minrange = maxrange;
    maxrange = temp;
  }
  if (minrange < param_lengthLower[inds[2]][inds[3]])
    minrange = param_lengthLower[inds[2]][inds[3]];
  if (maxrange > param_lengthUpper[inds[2]][inds[3]])
    maxrange = param_lengthUpper[inds[2]][inds[3]];

  if (minrange > maxrange)  // This case may happen if triangular boundary is
                            // tighter than tetrahedral boundary.
    minrange = maxrange;

  param_lengthLower[inds[2]][inds[3]] = minrange;
  param_lengthLower[inds[3]][inds[2]] = minrange;
  param_lengthUpper[inds[2]][inds[3]] = maxrange;
  param_lengthUpper[inds[3]][inds[2]] = maxrange;
  param_length[inds[2]][inds[3]] = minrange;
  param_length[inds[3]][inds[2]] = minrange;
}
