#include <iostream>
// The Octree library resides in a single header file.
#include <vector>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

#include "src/obj_reader/obj_reader.cpp"
#include "Stopwatch.h"

#include <pangolin/pangolin.h>
using namespace std;
#include <glm/glm.hpp>


//octree from pcl
#include "../pcl/common/include/pcl/point_cloud.h"
#include "../pcl/octree/include/pcl/octree/octree_search.h"



GLuint objVerticiesVBO;
std::vector<glm::vec3> objVertices;

struct color {
    float r;
    float g;
    float b;
} colorR ;

struct Pointcloud {
    float x,y,z;
    float r[3];

};

std::vector<color> colors;

objReader pointcloud;
vector<glm::vec3> my_object;
// Any 3D vector implementation can be used, as long as it can be converted
// to float[3]. Here we use MyPoint.


objReader readObj(char* filename){
    objReader testobj;

    //char* filename = "Z6.obj";
    testobj.objLoadFile(filename);
    testobj.objLoadModel();
    cout<<"No. of vertices: " << testobj.nVertex << endl;

    return testobj;
}

struct CustomType
{
    CustomType()
        : x(0), y(0.0f) {}

    CustomType(int x, float y, std::string z)
        : x(x), y(y), z(z) {}

    int x;
    float y;
    std::string z;
};

std::ostream& operator<< (std::ostream& os, const CustomType& o){
    os << o.x << " " << o.y << " " << o.z;
    return os;
}

std::istream& operator>> (std::istream& is, CustomType& o){
    is >> o.x;
    is >> o.y;
    is >> o.z
;    return is;
}

void SampleMethod()
{
    std::cout << "You typed ctrl-r or pushed reset" << std::endl;
    glRotated (30, 30, 30, 30);
}


void draw(Pointcloud  Mycloud[] , size_t Mycloud_Size)
{
    // Load configuration data
    pangolin::ParseVarsFile("app.cfg");

    // Create OpenGL window in single line
    pangolin::CreateWindowAndBind("Main",1024  ,768);

    // 3D Mouse handler requires depth testing to be enabled
    glEnable(GL_DEPTH_TEST);

    // Define Camera Render Object (for view / scene browsing)
    pangolin::OpenGlRenderState s_cam(
                pangolin::ProjectionMatrix(640,480,420,420,320,240,0.1,1000),
                pangolin::ModelViewLookAt(-0,0.5,-3, 0,0,0, pangolin::AxisY)
                );

    const int UI_WIDTH = 180;

    // Add named OpenGL viewport to window and provide 3D Handler
    pangolin::View& d_cam = pangolin::CreateDisplay()
        .SetBounds(0.0, 1.0, pangolin::Attach::Pix(UI_WIDTH), 1.0, -640.0f/480.0f)
        .SetHandler(new pangolin::Handler3D(s_cam));

    // Add named Panel and bind to variables beginning 'ui'
    // A Panel is just a View with a default layout and input handling
    pangolin::CreatePanel("ui")
        .SetBounds(0.0, 1.0, 0.0, pangolin::Attach::Pix(UI_WIDTH));



    
    // Safe and efficient binding of named variables.
    // Specialisations mean no conversions take place for exact types
    // and conversions between scalar types are cheap.
    pangolin::Var<bool> a_button("ui.A_Button",false,false);
    pangolin::Var<double> a_double("ui.A_Double",3,0,5);
    pangolin::Var<int> an_int("ui.An_Int",2,0,5);
    pangolin::Var<double> a_double_log("ui.Log_scale var",3,1,1E4, true);
    pangolin::Var<bool> a_checkbox("ui.A_Checkbox",false,true);
    pangolin::Var<int> an_int_no_input("ui.An_Int_No_Input",2);
    pangolin::Var<CustomType> any_type("ui.Some_Type", CustomType(0,1.2f,"Hello") );

    pangolin::Var<bool> save_window("ui.Save_Window",false,false);
    pangolin::Var<bool> save_cube("ui.Save_Cube",false,false);

    pangolin::Var<bool> record_cube("ui.Record_Cube",false,false);


    // std::function objects can be used for Var's too. These work great with C++11 closures.
    pangolin::Var<std::function<void(void)> > reset("ui.Reset", SampleMethod);

    // Demonstration of how we can register a keyboard hook to alter a Var
    pangolin::RegisterKeyPressCallback(pangolin::PANGO_CTRL + 'b', pangolin::SetVarFunctor<double>("ui.A Double", 3.5));

    // Demonstration of how we can register a keyboard hook to trigger a method
    pangolin::RegisterKeyPressCallback(pangolin::PANGO_CTRL + 'r', SampleMethod);
    // Default hooks for exiting (Esc) and fullscreen (tab).


    cout << " OBJECT VERTICES ARE " <<(Mycloud_Size)/sizeof(Pointcloud) << endl;

    glGenBuffers(1, &objVerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, objVerticiesVBO);

    glBufferData(GL_ARRAY_BUFFER, (Mycloud_Size/sizeof(Pointcloud)) * sizeof(Pointcloud),
        &Mycloud[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //backround color
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    glPointSize(2);

    while( !pangolin::ShouldQuit() )
    {
        // Clear entire screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if( pangolin::Pushed(a_button) )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
          std::cout << "You Pushed a button!" << std::endl;

        if( a_checkbox )
          an_int = (int)a_double;

        if( !any_type->z.compare("robot"))
          any_type = CustomType(1,2.3f,"Boogie");

        an_int_no_input = an_int;

        if( pangolin::Pushed(save_window) )
          pangolin::SaveWindowOnRender("window");

        if( pangolin::Pushed(save_cube) )
          d_cam.SaveOnRender("cube");

        if( pangolin::Pushed(record_cube) )
          pangolin::DisplayBase().RecordOnRender("ffmpeg:[fps=50,bps=8388608,unique_filename]//screencap.avi");

        d_cam.Activate(s_cam);

        //pcl::PointCloud<pcl::PointXYZRGB>::Ptr ptCloud (new pcl::PointCloud<pcl::PointXYZRGB>);

        //PangoCloud cloud(pointcloud.get());

        glBindBuffer(GL_ARRAY_BUFFER, objVerticiesVBO);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        //glVertexPointer(3, GL_FLOAT, sizeof(glm::vec3), 0 );
        //glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(glm::vec3), (void *)sizeof(float) );


        
        //cout << &Mycloud[0].x << " rererere        " << &Mycloud[0].x << endl;

        glVertexPointer(3, GL_FLOAT, sizeof(Pointcloud), 0);

        glColorPointer(3, GL_FLOAT, sizeof(Pointcloud), (void *)(sizeof(float) * 3));


        //glColor3f(15.0,80.0,1.0);

        glDrawArrays(GL_POINTS, 0, objVertices.size());

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        /*
        pangolin::GlBuffer glxyz(pangolin::getlArrayBuffer, num_pts, GL_FLOAT, 3, GL_STATIC_DRAW);
        glxyz.Upload(somedata, 3*sizeof(float)*num_pts);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
        pangolin::RenderVbo(glxyz, GL_POINTS);
        */
        
        pangolin::FinishFrame();
    }

}

int main(int argc, char** argv)
{
    

    pointcloud = readObj(argv[1]);
    const int nPoints = pointcloud.nVertex;


    float sum_x = 0, sum_y = 0 , sum_z = 0 ;


    for (int i = 0; i < nPoints; ++i){

            float x = (float)pointcloud.vertexArray[i].x;
            float y = (float)pointcloud.vertexArray[i].y;
            float z = (float)pointcloud.vertexArray[i].z;

            sum_x = sum_x + x;
            sum_y = sum_y + y;
            sum_z = sum_z + z;


    }

    cout << "centroid of object is: " << sum_x/nPoints << "  " << sum_y/nPoints << "  " << sum_z/nPoints <<endl ;

    glm::vec3 centroid = glm::vec3(sum_x / nPoints , sum_y / nPoints , sum_z / nPoints);


    Pointcloud Mycloud[nPoints];

    // new cloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

    cloud->points.resize (nPoints);

    for (int i = 0; i < nPoints; ++i){

        float x = (float)pointcloud.vertexArray[i].x; 
        float y = (float)pointcloud.vertexArray[i].y;
        float z = (float)pointcloud.vertexArray[i].z;

        objVertices.push_back(glm::vec3(x - centroid.x , y - centroid.y, z - centroid.z + 12));

        Mycloud[i].x = x - centroid.x;
        Mycloud[i].y = y - centroid.y;
        Mycloud[i].z = z - centroid.z;

        Mycloud[i].r[0] =50;
        Mycloud[i].r[1] = 0;
        Mycloud[i].r[2] = 0;


        cloud->points[i].x = Mycloud[i].x;
        cloud->points[i].y = Mycloud[i].y;
        cloud->points[i].z = Mycloud[i].z;


    }  


    float resolution = 1.0f;
    pcl::octree::OctreePointCloudSearch<pcl::PointXYZ> octree (resolution);
    octree.setInputCloud (cloud);
    octree.addPointsFromInputCloud ();


    pcl::PointXYZ searchPoint, searchPoint1 ;

    searchPoint.x = Mycloud[200].x;
    searchPoint.y = Mycloud[200].y;
    searchPoint.z = Mycloud[200].z;
      // Neighbors within voxel search

    std::vector<int> pointIdxVec , pointIdxVec1;

    if (octree.voxelSearch (searchPoint, pointIdxVec))
    {
    std::cout << "Neighbors within voxel search at (" << searchPoint.x 
        << " " << searchPoint.y 
        << " " << searchPoint.z << ")" 
        << std::endl;
                  
        for (size_t i = 0; i < pointIdxVec.size (); ++i){

                Mycloud[pointIdxVec[i]].r[0] = 0;
                Mycloud[pointIdxVec[i]].r[1] = 0;
                Mycloud[pointIdxVec[i]].r[2] = 100;


                std::cout << "    " << cloud->points[pointIdxVec[i]].x 
                << " " << cloud->points[pointIdxVec[i]].y 
                << " " << cloud->points[pointIdxVec[i]].z << std::endl;
            }
    }

    searchPoint1.x = Mycloud[100].x;
    searchPoint1.y = Mycloud[100].y;
    searchPoint1.z = Mycloud[100].z;

    double start = stopwatch();

    if (octree.voxelSearch (searchPoint1, pointIdxVec1))
    {
    std::cout << "Neighbors within voxel search at (" << searchPoint1.x 
        << " " << searchPoint1.y 
        << " " << searchPoint1.z << ")" 
        << std::endl;
                  
        for (size_t i = 0; i < pointIdxVec1.size (); ++i){

                Mycloud[pointIdxVec1[i]].r[0] = 0;
                Mycloud[pointIdxVec1[i]].r[1] = 100;
                Mycloud[pointIdxVec1[i]].r[2] = 0;


                std::cout << "    " << cloud->points[pointIdxVec1[i]].x 
                << " " << cloud->points[pointIdxVec1[i]].y 
                << " " << cloud->points[pointIdxVec1[i]].z << std::endl;
            }
    }

    double T = stopwatch() - start;
    printf("Search of Point in %.5f sec.\n", T);

    //Draw Pointcloud

    draw(Mycloud , sizeof(Mycloud));
        
   
    return 0;
}
