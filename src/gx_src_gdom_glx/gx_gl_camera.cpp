<<<<<<< HEAD
#include <iostream>

#include <gx_gdom.h>
#include <gx_glm_utils.h>

// gx::fn - располагается в словаре gx::ns::code, а указатели на него, как
// на источник значения, разбросаны во всех выходящих (output) узлах.
// Таким образом, функция окраски в вычисленно (корректно, valid ) всего
// вывода должна выполняться самим gx::fn, так как только в нём есть
// информация о выходящих узлах и путях к ним. Аналогично вычислитель
// должен регистрироваться на события всех входящих узлов.

// need separate "view" field in camera, that depend "wpos"
// camera_view_calc( "view:m4:1" | "wpos:m4:1" )
struct camera_view_calc_t : gx::fn
{
    gx::m4 *mp_view;  // pointer to output GDOM field
    gx::m4 *mp_wpos;  // pointer to input GDOM field

    camera_view_calc_t(gx::ns *base) : gx::fn(base)
    {
        std::cout<<"++camera_view_calc_t at <" << this << ">"<<std::endl;
        get(&mp_wpos, "wpos");
        set(&mp_view, "view");
    }

    ~camera_view_calc_t()
    {
        std::cout<<"--camera_view_calc_t at <" << this << ">"<<std::endl;
    }

    virtual void calc()
    {
        if( fn_bind() )          // avoid GPF!
        {
            if( fn_calc() )      // check or recalc input
            {

                glm::mat4 camera_world_position(1.f);                 // camera world position
                glm::mat4 camera_view(1.f);                           // camera view transformation

                // load camera world position from gdom "wpos"
                set_glm_mat4_from_m4(camera_world_position, mp_wpos); // init glm-variable from gx-gdom-field

                // GetViewMatrix() const
                float* cwp = glm::value_ptr(camera_world_position);   // used as float[16]
                glm::vec3 camera_position(cwp[12], cwp[13], cwp[14]); // cwp translation;
                glm::vec3 z(cwp[ 2], cwp[ 6], cwp[10]);               // cwp z-vector;
                glm::vec3 look_at_point = camera_position + z;
                glm::vec3 up_vector      (cwp[ 1], cwp[ 5], cwp[ 9]); // cwp y-vector;

                camera_view = glm::lookAt(camera_position, look_at_point, up_vector);

                // save camera_view to gdom "view"
                set_m4_from_glm_mat4(mp_view, camera_view);

                // reset state to GOOD
                return gx::fn::calc_success();
            }
            else
            {
                return gx::fn::fn_calc_failed();
            }
        }
        else
        {
            return gx::fn::fn_bind_failed();
        }
    }
};

// camera_calc_view( "view:m4:1" | "wpos:m4:1" )
// camera_calc_proj( "proj:m4:1" | "size:f2:1", "fov:f1:1", "near:f1:1", "far:f1:1")
struct camera_calc_proj_fn_t : gx::fn
{
    //output
    gx::m4 *mp_proj;

    //input
    gx::f1 *mp_fov, *mp_near, *mp_far;
    gx::f2 *mp_size;

    camera_calc_proj_fn_t(gx::ns* base) : gx::fn(base) { //
        std::cout<<"++camera_calc_proj_fn_t at <" << this << ">"<<std::endl;

        // declare input
        get(&mp_fov , "fov" ); // register_self_as_getter(mp_fov , "fov" );
        get(&mp_near, "near"); // register_self_as_getter(mp_near, "near");
        get(&mp_far , "far" ); // register_self_as_getter(mp_far , "far" );
        get(&mp_size, "size"); // register_self_as_getter(mp_size, "size");

        // declare output
        set(&mp_proj, "proj"); // register_self_as_setter(mp_proj, "proj");
    }

    ~camera_calc_proj_fn_t() {
        std::cout<<"--camera_calc_proj_fn_t at <" << this << ">"<<std::endl;
    }

    virtual void calc()
    {
        if( fn_bind() )  // Check bind state, to avoid GPF "Global Protection Fault" on next steps
        {
            if( fn_calc() )
            {
                // GetProjectionMatrix()
                glm::mat4 camera_projection_matrix;
                
                float w = ( mp_size->data[0] > 1 ) ? mp_size->data[0] : 1;
                float h = ( mp_size->data[1] > 1 ) ? mp_size->data[1] : 1;
                float aspect = w / h;

                camera_projection_matrix = glm::perspective
                    ( mp_fov->data.at(0)    // 45.f   <= fov
                    , aspect                // width / height
                    , mp_near->data.at(0)   // 0.1f   <= near clip plane
                    , mp_far->data.at(0)    // 100.0f <= far clip plane
                    );
                
                // save camera_projection_matrix to gdom "proj"
                set_m4_from_glm_mat4( mp_proj, camera_projection_matrix );

                return gx::fn::calc_success();
            }
            return fn_calc_failed();
        }
        return fn_bind_failed();
    }
};

struct camera_calc_view_proj_fn_t : gx::fn
{
    //output
    gx::m4 *mp_view_proj;

    //input
    gx::m4 *mp_view, *mp_proj;

    camera_calc_view_proj_fn_t(gx::ns* base)
        :gx::fn(base)
        ,mp_view_proj(NULL)
        ,mp_view(NULL)
        ,mp_proj(NULL)
    {
        std::cout<<"++camera_calc_view_proj_fn_t<" << this << ">"<<std::endl;

        get( &mp_proj, "proj" );            // register_self_as_getter(mp_proj, "proj");
        get( &mp_view, "view" );            //  register_self_as_getter(mp_view, "view");

        set( &mp_view_proj, "view_proj" );  // register_self_as_setter(mp_view_proj, "view_proj");
    }

    ~camera_calc_view_proj_fn_t()
    {
        std::cout<<"--camera_calc_view_proj_fn_t<" << this << ">"<<std::endl;
    }

    virtual void calc()
    {
        // Calc a GetViewProjectionMatrix()
        // MVP = GetProjectionMatrix() * GetViewMatrix() * GetModelMatrix();
        // MVP = GetViewProjectionMatrix() * GetModelMatrix();
        if( fn_bind() ) // avoid GPF!
        {
            if( fn_calc() ) // calculate self input values, before self output
            {
                glm::mat4 view, proj, view_proj;

                // load calculated input from gdom:
                set_glm_mat4_from_m4(view, mp_view);  // load from gdom 'view'
                set_glm_mat4_from_m4(proj, mp_proj);  // load from gdom 'proj'

                // at last can calc output value!
                view_proj = proj * view;              // use temp var visible at debug time

                // save calculated output to gdom
                set_m4_from_glm_mat4( mp_view_proj, view_proj ); // save in gdom 'view_proj'

                // chande self DG state to GOOD, and invalidate:
                return calc_success();
            }
            return fn_calc_failed();
        }
        return fn_bind_failed();
    }
};

gx::fn* fn_camera_view_proj_factory(gx::ns* base, const char* name)
{
    camera_calc_view_proj_fn_t *p_calc; // gx::fn::result_t* logs = gx::fn::result();
    p_calc = new camera_calc_view_proj_fn_t( base );
    base->code[name] = p_calc;
    p_calc -> fn_bind();  // test bind  // p_calc -> calc();  // test calc
    return p_calc;
}

gx::fn* fn_camera_view_factory(gx::ns* base, const char* name)
{
    camera_view_calc_t* p_calc;  // gx::fn::result_t* logs = gx::fn::result();
    p_calc = new camera_view_calc_t( base );
    base->code[name] = p_calc;
    p_calc -> fn_bind();  // test bind    // p_calc -> calc();  // test calc
    return p_calc;
}

gx::fn* fn_camera_proj_factory(gx::ns* base, const char* name)
{
    camera_calc_proj_fn_t* p_calc; // gx::fn::result_t* logs = gx::fn::result();
    p_calc = new camera_calc_proj_fn_t( base );
    base->code[name] = p_calc;
    p_calc -> fn_bind();  // test bind  // p_calc -> calc();  // test calc
    return p_calc;
}
=======
#include <iostream>

#include <gx_gdom.h>
#include <gx_glm_utils.h>

// gx::fn - располагается в словаре gx::ns::code, а указатели на него, как
// на источник значения, разбросаны во всех выходящих (output) узлах.
// Таким образом, функция окраски в вычисленно (корректно, valid ) всего
// вывода должна выполняться самим gx::fn, так как только в нём есть
// информация о выходящих узлах и путях к ним. Аналогично вычислитель
// должен регистрироваться на события всех входящих узлов.

// need separate "view" field in camera, that depend "wpos"
struct camera_view_calc_t : gx::fn
{
    gx::m4 *mp_view;  // pointer to output GDOM field
    gx::m4 *mp_wpos;  // pointer to input GDOM field

    camera_view_calc_t(gx::ns *base)
        : gx::fn(base)
    {
        std::cout<<"++camera_view_calc_t at <" << this << ">"<<std::endl;

        // input - output
        get(&mp_wpos, "wpos");
        set(&mp_view, "view");
    }

    ~camera_view_calc_t()
    {
        std::cout<<"--camera_view_calc_t at <" << this << ">"<<std::endl;
    }


    virtual void calc()
    {
        if( fn_bind() )          // avoid GPF!
        {
            if( fn_calc() )      // check or recalc input
            {

                glm::mat4 camera_world_position(1.f);                 // camera world position
                glm::mat4 camera_view(1.f);                           // camera view transformation

                // load camera world position from gdom "wpos"
                set_glm_mat4_from_m4(camera_world_position, mp_wpos); // init glm-variable from gx-gdom-field

                // GetViewMatrix() const
                float* cwp = glm::value_ptr(camera_world_position);   // used as float[16]
                glm::vec3 camera_position(cwp[12], cwp[13], cwp[14]); // cwp translation;
                glm::vec3 z(cwp[ 2], cwp[ 6], cwp[10]);               // cwp z-vector;
                glm::vec3 look_at_point = camera_position + z;
                glm::vec3 up_vector      (cwp[ 1], cwp[ 5], cwp[ 9]); // cwp y-vector;

                camera_view = glm::lookAt(camera_position, look_at_point, up_vector);

                // save camera_view to gdom "view"
                set_m4_from_glm_mat4(mp_view, camera_view);

                // reset state to GOOD
                return gx::fn::calc_success();
            }
            else
            {
                return gx::fn::fn_calc_failed();
            }
        }
        else
        {
            return gx::fn::fn_bind_failed();
        }
    }
};


// need separate "proj" field in camera, that depend
//   "size", "fov", "near", "far"
// const char* send = "m4:proj"; //const char* recv = "f2:size, f1:fov, f1:near, f1:far";

struct camera_calc_proj_fn_t : gx::fn
{
    //output
    gx::m4 *mp_proj;

    //input
    gx::f1 *mp_fov, *mp_near, *mp_far;
    gx::f2 *mp_size;

    camera_calc_proj_fn_t(gx::ns* base) : gx::fn(base) { //
        std::cout<<"++camera_calc_proj_fn_t at <" << this << ">"<<std::endl;

        // declare input
        get(&mp_fov , "fov" ); // register_self_as_getter(mp_fov , "fov" );
        get(&mp_near, "near"); // register_self_as_getter(mp_near, "near");
        get(&mp_far , "far" ); // register_self_as_getter(mp_far , "far" );
        get(&mp_size, "size"); // register_self_as_getter(mp_size, "size");

        // declare output
        set(&mp_proj, "proj"); // register_self_as_setter(mp_proj, "proj");
    }

    ~camera_calc_proj_fn_t() {
        std::cout<<"--camera_calc_proj_fn_t at <" << this << ">"<<std::endl;
    }

    virtual void calc()
    {
        if( fn_bind() )  // Check bind state, to avoid GPF "Global Protection Fault" on next steps
        {
            if( fn_calc() )
            {
                // GetProjectionMatrix()
                glm::mat4 camera_projection_matrix;
                
                float w = ( mp_size->data[0] > 1 ) ? mp_size->data[0] : 1;
                float h = ( mp_size->data[1] > 1 ) ? mp_size->data[1] : 1;
                float aspect = w / h;

                camera_projection_matrix = glm::perspective
                    ( mp_fov->data.at(0)    // 45.f   <= fov
                    , aspect                // width / height
                    , mp_near->data.at(0)   // 0.1f   <= near clip plane
                    , mp_far->data.at(0)    // 100.0f <= far clip plane
                    );
                
                // save camera_projection_matrix to gdom "proj"
                set_m4_from_glm_mat4( mp_proj, camera_projection_matrix );

                return gx::fn::calc_success();
            }
            return fn_calc_failed();
        }
        return fn_bind_failed();
    }
};

struct camera_calc_view_proj_fn_t : gx::fn
{

    //output
    gx::m4 *mp_view_proj;

    //input
    gx::m4 *mp_view, *mp_proj;

    camera_calc_view_proj_fn_t(gx::ns* base)
        :gx::fn(base)
        ,mp_view_proj(NULL)
        ,mp_view(NULL)
        ,mp_proj(NULL)
    {
        std::cout<<"++camera_calc_view_proj_fn_t<" << this << ">"<<std::endl;

        get( &mp_proj, "proj" );            // register_self_as_getter(mp_proj, "proj");
        get( &mp_view, "view" );            //  register_self_as_getter(mp_view, "view");

        set( &mp_view_proj, "view_proj" );  // register_self_as_setter(mp_view_proj, "view_proj");
    }

    ~camera_calc_view_proj_fn_t()
    {
        std::cout<<"--camera_calc_view_proj_fn_t<" << this << ">"<<std::endl;
    }

    virtual void calc()
    {
        // Calc a GetViewProjectionMatrix()
        // MVP = GetProjectionMatrix() * GetViewMatrix() * GetModelMatrix();
        // MVP = GetViewProjectionMatrix() * GetModelMatrix();
        if( fn_bind() ) // avoid GPF!
        {
            if( fn_calc() ) // calculate self input values, before self output
            {
                // Check input calculation's
                // if not success can return w/o state changed,
                // or can write defaults, but also w/o state changed!
                //if( get_calc(mp_view) ) {  return gx::fn::get_calc_failed( mp_view ); };
                //if( get_calc(mp_proj) ) {  return gx::fn::get_calc_failed( mp_proj ); };

                glm::mat4 view, proj, view_proj;

                // load calculated input from gdom:
                set_glm_mat4_from_m4(view, mp_view);  // load from gdom 'view'
                set_glm_mat4_from_m4(proj, mp_proj);  // load from gdom 'proj'

                // at last can calc output value!
                view_proj = proj * view;              // use temp var visible at debug time

                // save calculated output to gdom
                set_m4_from_glm_mat4( mp_view_proj, view_proj ); // save in gdom 'view_proj'

                // chande self DG state to GOOD:
                return calc_success();
            }
            return fn_calc_failed();
        }
        return fn_bind_failed();
    }
};


gx::fn* fn_camera_view_proj_factory(gx::ns* base, const char* name)
{
    camera_calc_view_proj_fn_t *p_calc; // gx::fn::result_t* logs = gx::fn::result();
    p_calc = new camera_calc_view_proj_fn_t( base );
    base->code[name] = p_calc;
    p_calc -> fn_bind();  // test bind  // p_calc -> calc();  // test calc
    return p_calc;
}


gx::fn* fn_camera_view_factory(gx::ns* base, const char* name)
{
    camera_view_calc_t* p_calc;  // gx::fn::result_t* logs = gx::fn::result();
    p_calc = new camera_view_calc_t( base );
    base->code[name] = p_calc;
    p_calc -> fn_bind();  // test bind    // p_calc -> calc();  // test calc
    return p_calc;
}



gx::fn* fn_camera_proj_factory(gx::ns* base, const char* name)
{
    camera_calc_proj_fn_t* p_calc; // gx::fn::result_t* logs = gx::fn::result();
    p_calc = new camera_calc_proj_fn_t( base );
    base->code[name] = p_calc;
    p_calc -> fn_bind();  // test bind  // p_calc -> calc();  // test calc
    return p_calc;
}
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
