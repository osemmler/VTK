/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkIntersectionPolyDataFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkIntersectionPolyDataFilter
// .SECTION Description
//
// vtkIntersectionPolyDataFilter computes the intersection between two
// vtkPolyData objects. The first output is a set of lines that marks
// the intersection of the input vtkPolyData objects. This contains five
// different attached data arrays:
//
// SurfaceID: Point data array that contatins information about the orgin
// surface of each point
//
// Input0CellID: Cell data array that says the contains the original
// cell id number on the first input mesh
//
// Input1CellID: Cell data array that says the contains the original
// cell id number on the second input mesh
//
// NewCell0ID: Cell data array that contains information about which cells
// of the remeshed first input surface it touches (If split)
//
// NewCell1ID: Cell data array that contains information about which cells
// on the remeshed second input surface it touches (If split)
//
// The second and third outputs are the first and second input vtkPolyData,
// respectively. Optionally, the two output vtkPolyData can be split
// along the intersection lines by remeshing. Optionally, the surface
// can be cleaned and checked at the end of the remeshing.
// If the meshes are split, The output vtkPolyDatas contain three possible
// data arrays:
//
// BoundaryPoints: This is a boolean indicating whether or not the point is
// on the boundary of the two input objects
//
// BadTri: If the surface is cleaned and checked, this is a cell data array
// indicating whether or not the cell has edges with multiple neighbors
// A manifold surface will have 0 everywhere for this array!
//
// FreeEdge: If the surface is cleaned and checked, this is a cell data array
// indicating if the cell has any free edges. A watertight surface will have
// 0 everywhere for this array!
/** @file vtkIntersectionPolyDataFilter.cxx
 *  @brief This is a filter that performs the intersection of surfaces
 *  @author Adam Updegrove
 *  @author updega2@gmail.com
 */

#ifndef vtkIntersectionPolyDataFilter_h
#define vtkIntersectionPolyDataFilter_h

#include "vtkFiltersGeneralModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class VTKFILTERSGENERAL_EXPORT vtkIntersectionPolyDataFilter :
        public vtkPolyDataAlgorithm
{
public:
  static vtkIntersectionPolyDataFilter *New();
  vtkTypeMacro(vtkIntersectionPolyDataFilter, vtkPolyDataAlgorithm);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Integer describing the number of intersection points and lines
  vtkGetMacro(NumberOfIntersectionPoints, int);
  vtkGetMacro(NumberOfIntersectionLines, int);

  // Description:
  // If on, the second output will be the first input mesh split by the
  // intersection with the second input mesh. Defaults to on.
  vtkGetMacro(SplitFirstOutput, int);
  vtkSetMacro(SplitFirstOutput, int);
  vtkBooleanMacro(SplitFirstOutput, int);

  // Description:
  // If on, the third output will be the second input mesh split by the
  // intersection with the first input mesh. Defaults to on.
  vtkGetMacro(SplitSecondOutput, int);
  vtkSetMacro(SplitSecondOutput, int);
  vtkBooleanMacro(SplitSecondOutput, int);

  // Description:
  // If on, the output split surfaces will contain information about which
  // points are on the boundary of the two inputs. Default: ON
  vtkGetMacro(ApplyBoundaryPointArray, int);
  vtkSetMacro(ApplyBoundaryPointArray, int);
  vtkBooleanMacro(ApplyBoundaryPointArray, int);

  // Description:
  // If on, the normals of the input will be checked. Default: OFF
  vtkGetMacro(CheckInput, int);
  vtkSetMacro(CheckInput, int);
  vtkBooleanMacro(CheckInput, int);

  // Description:
  // If on, the output remeshed surfaces will be checked for bad cells and
  // free edges. Default: ON
  vtkGetMacro(CheckMesh, int);
  vtkSetMacro(CheckMesh, int);
  vtkBooleanMacro(CheckMesh, int);

  // Description:
  // Check the status of the filter after update. If the status is zero,
  // there was an error in the operation. If status is one, everything
  // went smoothly
  vtkGetMacro(Status, int);

  // Description:
  // The tolerance for geometric tests in the filter
  vtkGetMacro(Tolerance, double);
  vtkSetMacro(Tolerance, double);

  // Description:
  // Given two triangles defined by points (p1, q1, r1) and (p2, q2,
  // r2), returns whether the two triangles intersect. If they do,
  // the endpoints of the line forming the intersection are returned
  // in pt1 and pt2. The parameter coplanar is set to 1 if the
  // triangles are coplanar and 0 otherwise.
  static int TriangleTriangleIntersection(double p1[3], double q1[3],
                                          double r1[3], double p2[3],
                                          double q2[3], double r2[3],
                                          int &coplanar, double pt1[3],
                                          double pt2[3], double surfaceid[2],
                                          double tolerance);

  // Description:
  // Function to clean and check the output surfaces for bad triangles and
  // free edges
  static void CleanAndCheckSurface(vtkPolyData *pd, double stats[2],
                  double tolerance);

  // Description:
  // Function to clean and check the inputs
  static void CleanAndCheckInput(vtkPolyData *pd, double tolerance);


protected:
  vtkIntersectionPolyDataFilter();  //Constructor
  ~vtkIntersectionPolyDataFilter();  //Destructor

  int RequestData(vtkInformation*, vtkInformationVector**,
                  vtkInformationVector*);  //Update
  int FillInputPortInformation(int, vtkInformation*); //Input,Output

private:
  vtkIntersectionPolyDataFilter(const vtkIntersectionPolyDataFilter&);  // Not implemented
  void operator=(const vtkIntersectionPolyDataFilter&);  // Not implemented

  int NumberOfIntersectionPoints;
  int NumberOfIntersectionLines;
  int SplitFirstOutput;
  int SplitSecondOutput;
  int ApplyBoundaryPointArray;
  int CheckMesh;
  int CheckInput;
  int Status;
  double Tolerance;

  class Impl;  //Implementation class
};


#endif // vtkIntersectionPolyDataFilter_h
