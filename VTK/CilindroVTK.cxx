#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h> // Para esferas
#include <vtkTransform.h> // Para pos


int main(int, char*[]) {
    // Colores
    vtkNew<vtkNamedColors> colors;

    // CILINDRO
    // Forma
    // Esto genera los datos matemáticos puros (vértices y polígonos)
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetHeight(3.0); // seteando geometria
    cylinder->SetResolution(120); // RESOLUCION DEL RENDERIZADO

    // Convierte los datos matemáticos en datos gráficos
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    // geometría + propiedades visuales
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    // ESFERA
    vtkNew<vtkSphereSource> sphere;
    sphere->SetRadius(0.5);
    sphere->SetPhiResolution(20);
    sphere->SetThetaResolution(20);

    vtkNew<vtkTransform> transform;
    transform->Translate(0.0, 0.0, 1.5);

    vtkNew<vtkPolyDataMapper> sphereMapper;
    sphereMapper->SetInputConnection(sphere->GetOutputPort());

    vtkNew<vtkActor> sphereActor;
    sphereActor->SetMapper(sphereMapper);
    sphereActor->SetUserTransform(transform); // Aplica la posición
    sphereActor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData()); // Color distinto para diferenciarla

    // Crea la escena y añade los actores
    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(cylinderActor); // llamando a los protegonistas
    renderer->AddActor(sphereActor); // llamando a la esfera
    renderer->SetBackground(colors->GetColor3d("MidnightBlue").GetData());

    // Ventana de renderizado
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->SetSize(600, 600);
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("Primer Ejemplo VTK - Cilindro");

    // Permite usar el raton
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // 8. Iniciar el renderizado y el bucle de eventos del ratón
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
