<<<<<<< HEAD
#ifndef GX_GLX_GDOM_H
#define GX_GLX_GDOM_H

#include <iostream>
#include <gx_gdom.h>
#include <gx_gdom_json_dump.h>
#include <gx_glm_utils.h>

#define GX_GL_ASPECT 1000   // GDOM ASPECT GRAPHICS

namespace gx
{
    struct gl: gx::fn  // OpenGL aspect extend DOM model with self-generated specific data
    {
        // AI must change draw order, but AI-aspect can change only DOM nodes, not gl-specific nodes;
        // AI must change objects meshes, shaders and visibility, use only DOM-api, not gl-specific;
        // So we need some place at DOM-node-side used by aspects, like PHISICS, SOUND, NETWORK etc.
        struct abstract_painter;  // is a drawer - functor with automatic variables, not some object with
        // multiple referenced shared data, scene graph is object with shared data


        struct draw;   // gx::ns* drawable item, use current view-projection, model-view matrices, and ptr to shading group;
        // On interact with this type, gl-visitor invocate self paint method (depended detected GL driver);

        struct sgxu;   // if entity with data inside removed from active draw, is not some as GPU-side resource removed too;
        struct sgxa;   // if entity with buffer inside removed from active draw, is not some as GPU side resource removed too;
        struct prog;   // need fragment and vertex shaders sources, both stored in DOM as u0-value with code or DOM-path;
        struct shgr;   // shading group must bind all "gxa" & all "gxu" for concrete "prog", and realise draw arrays or VBO;
        struct proj;   // active view-projection m4, depends selected camera
        struct tran;   // world or additional model-view transformation
        struct zord;   // z-order - use any gx::ns as self root; use root:add(xx) root:rem(xx) to manage item's decor

        struct proc
        {
            void operator()(gx::fn*o) { return (GX_GL_ASPECT == o->aid)?((gx::gl*)o)->on(this):on(o); }
            virtual void on(gx::fn*)=0;
            virtual void on(gx::gl::zord*)=0;
            virtual void on(gx::gl::draw*)=0;
            virtual void on(gx::gl::sgxu*)=0;
            virtual void on(gx::gl::sgxa*)=0;
            virtual void on(gx::gl::prog*)=0;
            virtual void on(gx::gl::shgr*)=0;
            virtual void on(gx::gl::proj*)=0;
            virtual void on(gx::gl::tran*)=0;
        };
        virtual void on(gx::gl::proc*)=0;
        virtual~gl(){}
        //
        void calc(){ return gx::fn::calc_success_no_changes(); }
        gl(gx::ns* mp_base):fn(mp_base){aid = GX_GL_ASPECT;}
    };
}  // namespace gx

struct gx::gl::sgxu: gx::gl
{
    void on(gx::gl::proc* p){p->on(this);}
};

struct gx::gl::sgxa: gx::gl
{
    // 1. Attribute variable - need source gx::fv, gx::iv or gx::bv data to GPU bind
    // 2. Must be signaled about source "set" or "del"
    // 3. On self destroy must send self GPU-side "id" to gx-gl-garbage-collector
    // 4. Must have default value, if real value are not ready or has incompatible
    //    size or other invalid property.
    void on(gx::gl::proc* p){p->on(this);}
};

struct gx::gl::prog: gx::gl
{
    // Помним, что это потомок gx::fn, и что он имеет некоторые свойства реализуемые
    // только при наличии GPU устройства. А это, в свою очередь, говорит о том, что
    // фабрику этой инстанции функтора "gx-gl-prog" должен модифицировать детектор
    // наличия GPU, детектор типа GPU, и детектор наличия расширений этого GPU.

    // При этом, если объект, к примеру массив vec3 позиций вершин, в списке указан,
    // gl-program-vars["gx_gxa_position"] == NULL;    // основание для поиска в "mesh"
    // gl-drawer-curr-mesh["gx_gxa_position"] == (gx::f3*) -> code["gx-gl-gxa"] n/a;
    // gl-program-vars["gx_gxa_position"] == gx::gl::sgxa* ? gpu_id ? attribute_enabled ? -> DRAW
    //                                                     |        |                   | -> next
    //                                                     |        |-> load from gx::f3* to GPU
    //                                                     |-> find in curr mesh

    // 1. Depend get edges: Have vertex and fragment shaders as must:
    gx::u0* vertex_shader_source;    // get calc some u0 +hr +dg incref and dg
    gx::u0* fragment_shader_source;  // get calc some u0 +hr +dg incref and dg
    // 2. Has list of all sgxu/sgxa, each of them are based on gdom item, so
    //    we must bind and enable them all to gpu-program, before call draw.
    //    At gl-program-bind-variable-time number of vertices can be modified
    //    crop them to minimal attributes array size from attributes arrays.
    std::map< std::string, sgxu* > m_uniform;
    std::map< std::string, sgxa* > m_attributes;
    void on(gx::gl::proc* p){p->on(this);}
};

struct gx::gl::shgr: gx::gl
{
    void on(gx::gl::proc* p){p->on(this);}

    // hold mesh and prog, depends:
    gx::u0* mp_path_to_mesh_gdom_node;  // <-self-base-"mesh-path"
    gx::u0* mp_path_to_prog_gdom_node;  // <-self-base-"prog-path"

    gx::ns* mp_mesh_dict;  // -> self-base-"mesh" on init, invalidate, del, fail = NULL
    gx::ns* mp_shdr_dict;  // -> self-base-"prog" on init, invalidate, del, fail = NULL

    // self very fast hash to draw
    // mesh, prog, all vars, except position and projection
    std::map<std::string, gx::gl::sgxa*> m_vertices_attributes;  // vertex_attribies
    std::map<std::string, gx::gl::sgxu*> m_programm_uniform;  //
    gx::gl::prog* mp_shader_programm;

};

struct gx::gl::proj: gx::gl
{
    void on(gx::gl::proc* p){p->on(this);}
    proj(gx::ns* mp_base):gx::gl(mp_base){}
};

struct gx::gl::tran: gx::gl
{
    void on(gx::gl::proc* p){p->on(this);}
};

struct gx::gl::draw: gx::gl  // gl-aspect-decorator
{
    //"tran" has own position matrix and "zord or draw child"
    //
    void on(gx::gl::proc* p){p->on(this);}
};

struct gx::gl::abstract_painter {
    std::ostream* mp_log;
    abstract_painter():mp_log(NULL) {
        if(mp_log){ (*mp_log)<<"++gx::gl::abstract_painter<"<<this<<">"<<std::endl; }
    }
    virtual ~abstract_painter(){
        if(mp_log){ (*mp_log)<<"--gx::gl::abstract_painter<"<<this<<">"<<std::endl; }
    }
};

struct gx::gl::zord: gx::gl  // gl-aspect-decorator
{
    // gx::fn properties
    //   -self- use any gx::ns::code slot as self path base
    //   -iget- use gx::ns* named "zord" as input, has defaults on bind error
    //   -oset- NONE ( powered by gl-paint )
    // minimal usage example:
    //   $Camera$ m4 proj;  // set - is change view-projection
    //   $Camera$ ns zord;  // set - is change all gx-ns for some other gx-ns
    //   $Camera$ code zord = gx::gl::zord($Camera$, "zord");

    gx::ns* mp_z_order;  // usually only one zord for one camera projection and position

    zord(gx::ns* mp_base):gx::gl(mp_base)
    {
        get(&mp_z_order,"zord");
    }
    void on(gx::gl::proc* p){p->on(this);}


//virtual bool fn_bind()
//    {
//        gx::bind_ns(&mp_z_order, mp_base-> at( "zord" ));
//        register_self_as_getter(mp_z_order, "zord");
//        return gx::fn::bind_success();
//    }

    virtual void calc()
    {
        // TODO: Copy calc method, use painting last called calc / clear last called on success // - for trace fail
        if(fn_bind())
        {
            if(fn_calc())
            {
                return gx::fn::calc_success(); //invalidate
            }
            // TODO: Can call calc_trace method: is a method like calc, but collect last calc error info
            return gx::fn::fn_calc_failed();
        }
        // TODO: Call bind_trace method: is a method like bind method, but collect last calc error info
        return gx::fn::fn_bind_failed();
    }
};

#endif // GX_GLX_GDOM_H
=======
#ifndef GX_GLX_GDOM_H
#define GX_GLX_GDOM_H

#include <iostream>
#include <gx_gdom.h>
#include <gx_gdom_json_dump.h>
#include <gx_glm_utils.h>

#define GX_GL_ASPECT 1000   // GDOM ASPECT GRAPHICS

namespace gx
{
    struct gl: gx::fn  // OpenGL aspect extend DOM model with self-generated specific data
    {
        // AI must change draw order, but AI-aspect can change only DOM nodes, not gl-specific nodes;
        // AI must change objects meshes, shaders and visibility, use only DOM-api, not gl-specific;
        // So we need some place at DOM-node-side used by aspects, like PHISICS, SOUND, NETWORK etc.
        struct abstract_painter;  // is a drawer - functor with automatic variables, not some object with
        // multiple referenced shared data, scene graph is object with shared data


        struct draw;   // gx::ns* drawable item, use current view-projection, model-view matrices, and ptr to shading group;
        // On interact with this type, gl-visitor invocate self paint method (depended detected GL driver);

        struct sgxu;   // if entity with data inside removed from active draw, is not some as GPU-side resource removed too;
        struct sgxa;   // if entity with buffer inside removed from active draw, is not some as GPU side resource removed too;
        struct prog;   // need fragment and vertex shaders sources, both stored in DOM as u0-value with code or DOM-path;
        struct shgr;   // shading group must bind all "gxa" & all "gxu" for concrete "prog", and realise draw arrays or VBO;
        struct proj;   // active view-projection m4, depends selected camera
        struct tran;   // world or additional model-view transformation
        struct zord;   // z-order - use any gx::ns as self root; use root:add(xx) root:rem(xx) to manage item's decor

        struct proc
        {
            void operator()(gx::fn*o) { return (GX_GL_ASPECT == o->aid)?((gx::gl*)o)->on(this):on(o); }
            virtual void on(gx::fn*)=0;
            virtual void on(gx::gl::zord*)=0;
            virtual void on(gx::gl::draw*)=0;
            virtual void on(gx::gl::sgxu*)=0;
            virtual void on(gx::gl::sgxa*)=0;
            virtual void on(gx::gl::prog*)=0;
            virtual void on(gx::gl::shgr*)=0;
            virtual void on(gx::gl::proj*)=0;
            virtual void on(gx::gl::tran*)=0;
        };
        virtual void on(gx::gl::proc*)=0;
        virtual~gl(){}
        //
        void calc(){ return gx::fn::calc_success_no_changes(); }
        gl(gx::ns* mp_base):fn(mp_base){aid = GX_GL_ASPECT;}
    };
}  // namespace gx

struct gx::gl::sgxu: gx::gl
{
    void on(gx::gl::proc* p){p->on(this);}
};

struct gx::gl::sgxa: gx::gl
{
    // 1. Attribute variable - need source gx::fv, gx::iv or gx::bv data to GPU bind
    // 2. Must be signaled about source "set" or "del"
    // 3. On self destroy must send self GPU-side "id" to gx-gl-garbage-collector
    // 4. Must have default value, if real value are not ready or has incompatible
    //    size or other invalid property.
    void on(gx::gl::proc* p){p->on(this);}
};

struct gx::gl::prog: gx::gl
{
    // Помним, что это потомок gx::fn, и что он имеет некоторые свойства реализуемые
    // только при наличии GPU устройства. А это, в свою очередь, говорит о том, что
    // фабрику этой инстанции функтора "gx-gl-prog" должен модифицировать детектор
    // наличия GPU, детектор типа GPU, и детектор наличия расширений этого GPU.

    // При этом, если объект, к примеру массив vec3 позиций вершин, в списке указан,
    // gl-program-vars["gx_gxa_position"] == NULL;    // основание для поиска в "mesh"
    // gl-drawer-curr-mesh["gx_gxa_position"] == (gx::f3*) -> code["gx-gl-gxa"] n/a;
    // gl-program-vars["gx_gxa_position"] == gx::gl::sgxa* ? gpu_id ? attribute_enabled ? -> DRAW
    //                                                     |        |                   | -> next
    //                                                     |        |-> load from gx::f3* to GPU
    //                                                     |-> find in curr mesh

    // 1. Depend get edges: Have vertex and fragment shaders as must:
    gx::u0* vertex_shader_source;    // get calc some u0 +hr +dg incref and dg
    gx::u0* fragment_shader_source;  // get calc some u0 +hr +dg incref and dg
    // 2. Has list of all sgxu/sgxa, each of them are based on gdom item, so
    //    we must bind and enable them all to gpu-program, before call draw.
    //    At gl-program-bind-variable-time number of vertices can be modified
    //    crop them to minimal attributes array size from attributes arrays.
    std::map< std::string, sgxu* > m_uniform;
    std::map< std::string, sgxa* > m_attributes;
    void on(gx::gl::proc* p){p->on(this);}
};

struct gx::gl::shgr: gx::gl
{
    void on(gx::gl::proc* p){p->on(this);}

    // hold mesh and prog, depends:
    gx::u0* mp_path_to_mesh_gdom_node;  // <-self-base-"mesh-path"
    gx::u0* mp_path_to_prog_gdom_node;  // <-self-base-"prog-path"

    gx::ns* mp_mesh_dict;  // -> self-base-"mesh" on init, invalidate, del, fail = NULL
    gx::ns* mp_shdr_dict;  // -> self-base-"prog" on init, invalidate, del, fail = NULL

    // self very fast hash to draw
    // mesh, prog, all vars, except position and projection
    std::map<std::string, gx::gl::sgxa*> m_vertices_attributes;  // vertex_attribies
    std::map<std::string, gx::gl::sgxu*> m_programm_uniform;  //
    gx::gl::prog* mp_shader_programm;

};

struct gx::gl::proj: gx::gl
{
    void on(gx::gl::proc* p){p->on(this);}
    proj(gx::ns* mp_base):gx::gl(mp_base){}
};

struct gx::gl::tran: gx::gl
{
    void on(gx::gl::proc* p){p->on(this);}
};

struct gx::gl::draw: gx::gl  // gl-aspect-decorator
{
    //"tran" has own position matrix and "zord or draw child"
    //
    void on(gx::gl::proc* p){p->on(this);}
};

struct gx::gl::abstract_painter {
    std::ostream* mp_log;
    abstract_painter():mp_log(NULL) {
        if(mp_log){ (*mp_log)<<"++gx::gl::abstract_painter<"<<this<<">"<<std::endl; }
    }
    virtual ~abstract_painter(){
        if(mp_log){ (*mp_log)<<"--gx::gl::abstract_painter<"<<this<<">"<<std::endl; }
    }
};

struct gx::gl::zord: gx::gl  // gl-aspect-decorator
{
    // gx::fn properties
    //   -self- use any gx::ns::code slot as self path base
    //   -iget- use gx::ns* named "zord" as input, has defaults on bind error
    //   -oset- NONE ( powered by gl-paint )
    // minimal usage example:
    //   $Camera$ m4 proj;  // set - is change view-projection
    //   $Camera$ ns zord;  // set - is change all gx-ns for some other gx-ns
    //   $Camera$ code zord = gx::gl::zord($Camera$, "zord");

    gx::ns* mp_z_order;  // usually only one zord for one camera projection and position

    zord(gx::ns* mp_base):gx::gl(mp_base)
    {
        get(&mp_z_order,"zord");
    }
    void on(gx::gl::proc* p){p->on(this);}


//virtual bool fn_bind()
//    {
//        gx::bind_ns(&mp_z_order, mp_base-> at( "zord" ));
//        register_self_as_getter(mp_z_order, "zord");
//        return gx::fn::bind_success();
//    }

    virtual void calc()
    {
        // TODO: Copy calc method, use painting last called calc / clear last called on success // - for trace fail
        if(fn_bind())
        {
            if(fn_calc())
            {
                return gx::fn::calc_success(); //invalidate
            }
            // TODO: Can call calc_trace method: is a method like calc, but collect last calc error info
            return gx::fn::fn_calc_failed();
        }
        // TODO: Call bind_trace method: is a method like bind method, but collect last calc error info
        return gx::fn::fn_bind_failed();
    }
};

#endif // GX_GLX_GDOM_H
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
