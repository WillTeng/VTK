/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestPolarAxes.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .SECTION Thanks
// This test was written by Philippe P�bay, Kitware SAS 2011

#include "vtkBYUReader.h"
#include "vtkCamera.h"
#include "vtkPolarAxesActor.h"
#include "vtkLight.h"
#include "vtkLODActor.h"
#include "vtkNew.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkTestUtilities.h"
#include "vtkTextProperty.h"

//----------------------------------------------------------------------------
int TestPolarAxes( int argc, char * argv [] )
{
  vtkNew<vtkBYUReader> reader;
  char* fname = vtkTestUtilities::ExpandDataFileName( argc, argv, "Data/teapot.g" );
  reader->SetGeometryFileName( fname );
  delete [] fname;

  vtkNew<vtkPolyDataNormals> normals;
  normals->SetInputConnection( reader->GetOutputPort() );

  vtkNew<vtkPolyDataMapper> readerMapper;
  readerMapper->SetInputConnection( normals->GetOutputPort() );

  vtkNew<vtkLODActor> readerActor;
  readerActor->SetMapper( readerMapper.GetPointer() );
  readerActor->GetProperty()->SetDiffuseColor( .5, .8, .3 );

  vtkNew<vtkOutlineFilter> outline;
  outline->SetInputConnection(normals->GetOutputPort() );

  vtkNew<vtkPolyDataMapper> mapOutline;
  mapOutline->SetInputConnection(outline->GetOutputPort() );

  vtkNew<vtkActor> outlineActor;
  outlineActor->SetMapper( mapOutline.GetPointer() );
  outlineActor->GetProperty()->SetColor( 0.0, 0.0, 0.0 );

  vtkNew<vtkCamera> camera;
  camera->SetClippingRange( 1.0, 100.0 );
  camera->SetFocalPoint( 0.9, 1.0, 0.0 );
  camera->SetPosition( 2., 6., 13. );

  vtkNew<vtkLight> light;
  light->SetFocalPoint( 0.21406, 1.5, 0.0 );
  light->SetPosition( 7., 7., 4. );

  vtkNew<vtkRenderer> renderer;
  renderer->SetActiveCamera( camera.GetPointer() );
  renderer->AddLight( light.GetPointer() );

  // Update normals in order to get correct bounds for polar axes
  normals->Update();
  
  vtkNew<vtkPolarAxesActor> polaxes;
  polaxes->SetBounds( normals->GetOutput()->GetBounds() );
  polaxes->SetPole( .5, 1., 3. );
  polaxes->SetAutoScaleRadius( false );
  polaxes->SetMaximumRadius( 3. );
  polaxes->SetMaximumAngle( 210. );
  polaxes->SetNumberOfRadialAxes( 8 );
  polaxes->SetCamera( renderer->GetActiveCamera() );
  polaxes->SetRadialLabelFormat( "%6.1f" );
  polaxes->GetRadialAxesProperty()->SetColor( .0, .0, 1. );
  polaxes->GetPolarArcsProperty()->SetColor( 1., .0, 0. );
  polaxes->GetPolarAxisTitleTextProperty()->SetColor( 1., 0., 0. );
  polaxes->GetPolarAxisLabelTextProperty()->SetColor( 1., 0., 0. );
  polaxes->SetNumberOfPolarAxisTicks( 11 );
  polaxes->SetAutoSubdividePolarAxis( false );
  polaxes->SetScreenSize( 9.0 );

  vtkNew<vtkRenderWindow> renWin;
  renWin->SetMultiSamples( 0 );
  renWin->AddRenderer( renderer.GetPointer() );
  renWin->SetWindowName( "VTK - Polar Axes custom range" );
  renWin->SetSize( 600, 600 );

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow( renWin.GetPointer() );

  renderer->SetBackground( .8, .8, .8 );
  renderer->AddViewProp( readerActor.GetPointer() );
  renderer->AddViewProp( outlineActor.GetPointer() );
  renderer->AddViewProp( polaxes.GetPointer() );
  renWin->Render();

  int retVal = vtkRegressionTestImage( renWin.GetPointer() );
  if ( retVal == vtkRegressionTester::DO_INTERACTOR)
    {
    iren->Start();
    }

  return !retVal;
}