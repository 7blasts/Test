#ifndef GX_GLX_ABSTRACT_DRAWER_H
#define GX_GLX_ABSTRACT_DRAWER_H

#include <gx_glx_gdom.h>
#include <gx_gdom_json_dump.h>

struct abstract_drawer_visit_zord: gx::gl::proc
{
    std::ostream* mp_log;
    const char*   mp_tab;
    const char*   mp_key;
    abstract_drawer_visit_zord()
        :mp_log(NULL)
        ,mp_tab("  ")
        ,mp_key("NA")
    {
    }
    void on(gx::fn*      o){ if(mp_log) (*mp_log) << mp_tab << "zord-code \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
    void on(gx::gl::draw*o){ if(mp_log) (*mp_log) << mp_tab << "zord-draw \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
    void on(gx::gl::sgxu*o){ if(mp_log) (*mp_log) << mp_tab << "zord-sgxu \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
    void on(gx::gl::sgxa*o){ if(mp_log) (*mp_log) << mp_tab << "zord-sgxa \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
    void on(gx::gl::prog*o){ if(mp_log) (*mp_log) << mp_tab << "zord-prog \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
    void on(gx::gl::shgr*o){ if(mp_log) (*mp_log) << mp_tab << "zord-shdr \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
    void on(gx::gl::proj*o){
        if(mp_log) (*mp_log) << mp_tab << "zord-proj \"" << mp_key << "\" at <"<< o << ">" << std::endl;
    }
    void on(gx::gl::tran*o){ if(mp_log) (*mp_log) << mp_tab << "zord-tran \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
    void on(gx::gl::zord*o){
        const char* p_zord_tab = ">>";
        if(mp_log)
        {
            (*mp_log) << mp_tab << "zord \"" << mp_key << "\" at <"<< o << ">\n";

            if(o->fn_bind()) // OR, IF CLEAN INPUT NEEDED, "o->calc()" must be called
            {
                // draw is not actually calc, is only scanner started with own RTS loop;
                // z-ord not needed good calc state, because visit each item in z-ord get-ns "zord"
                // o->info( *mp_log, std::string(mp_tab) + p_zord_tab );
                gx::ns* pns = o->mp_z_order;
                (*mp_log) << mp_tab << p_zord_tab << " BEGIN gx::ns at<" << pns << ">\n";
                gx::ns::data_t::iterator curr = pns->data.begin(), stop = pns->data.end();
                for( ; curr != stop; ++curr)
                {
                    // inspect each xx->code has or not view-decor  gx::gl::nogl is also good result,
                    // if no result, gx::gl::nogl must be added before continue with next item.
                    (*mp_log) << mp_tab << p_zord_tab << "  +" << " gx::xx \"" << curr->first << "\" at<" << curr->second << ">" << std::endl;

                    gx::gdom_json_dump printer(mp_log, "--?--");
                    printer(curr->second);

                    (*mp_log) << std::endl;
                }
            }
            else
            {
                (*mp_log) << mp_tab << p_zord_tab << "  EXIT: PREVENT GPF BAD BIND STATE" << std::endl;
            }
            (*mp_log) << mp_tab << "zord \"" << mp_key << "\" at <" << o << ">\n";
        }
    }
};

// TODO: gl-aspect use trivial gx-dom with gx::xx nodes, but has some set of gl-specific "DG-reactors" derived from gx::fn.
//
struct abstract_drawer {  // not draw, only print GL commands order
    gx::ns* mp_root;
    std::ostream *mp_log;
    gx::gl::zord *mp_scene;

    abstract_drawer(gx::ns* root, std::ostream* log = NULL)
        :mp_root(root), mp_log(log)
    {
        if(mp_log) (*mp_log)<<"++abstract_draw<"<<this<<">"<<std::endl;
    }
    virtual ~abstract_drawer()
    {
        if(mp_log) (*mp_log)<<"--abstract_draw<"<<this<<">"<<std::endl;
    }
    virtual void paint()
    {
        if(mp_log) (*mp_log)<<"<GL-BEGIN : abstract_draw<"<<this<<">::paint()>"<<std::endl;
        std::map<std::string, gx::fn*>::iterator curr=mp_root->code.begin(), stop=mp_root->code.end();
        for(; curr != stop; ++curr) {
            struct inspect_t: gx::gl::proc{
                std::ostream* mp_log;
                const char*   mp_tab;
                const char*   mp_key;
                inspect_t()
                    :mp_log(NULL)
                    ,mp_tab("  ")
                    ,mp_key("NA")
                {
                }
                void on(gx::fn*      o){ if(mp_log) (*mp_log) << mp_tab << "code \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
                void on(gx::gl::draw*o){ if(mp_log) (*mp_log) << mp_tab << "draw \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
                void on(gx::gl::sgxu*o){ if(mp_log) (*mp_log) << mp_tab << "sgxu \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
                void on(gx::gl::sgxa*o){ if(mp_log) (*mp_log) << mp_tab << "sgxa \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
                void on(gx::gl::prog*o){ if(mp_log) (*mp_log) << mp_tab << "prog \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
                void on(gx::gl::shgr*o){ if(mp_log) (*mp_log) << mp_tab << "shdr \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
                void on(gx::gl::proj*o){ if(mp_log) (*mp_log) << mp_tab << "proj \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
                void on(gx::gl::tran*o){ if(mp_log) (*mp_log) << mp_tab << "tran \"" << mp_key << "\" at <"<< o << ">" << std::endl; }
                void on(gx::gl::zord*o){
                    const char* p_zord_tab = ">>";
                    if(mp_log)
                    {
                        (*mp_log) << mp_tab << "zord \"" << mp_key << "\" at <"<< o << ">\n";

                        if(o->fn_bind()) // OR, IF CLEAN INPUT NEEDED, "o->calc()" must be called
                        {
                            // draw is not actually calc, is only scanner started with own RTS loop;
                            // z-ord not needed good calc state, because visit each item in z-ord get-ns "zord"
                            // o->info( *mp_log, std::string(mp_tab) + p_zord_tab );
                            gx::ns* pns = o->mp_z_order;
                            (*mp_log) << mp_tab << p_zord_tab << " BEGIN gx::ns at<" << pns << ">\n";
                            gx::ns::data_t::iterator curr = pns->data.begin(), stop = pns->data.end();
                            for( ; curr != stop; ++curr)
                            {
                                // inspect each xx->code has or not view-decor  gx::gl::nogl is also good result,
                                // if no result, gx::gl::nogl must be added before continue with next item.
                                (*mp_log) << mp_tab << p_zord_tab << "  +" << " gx::xx \"" << curr->first << "\" at<" << curr->second << ">" << std::endl;

                                gx::gdom_json_dump printer(mp_log, "--?--");
                                printer(curr->second);

                                (*mp_log) << std::endl;
                            }
                        }
                        else
                        {
                            (*mp_log) << mp_tab << p_zord_tab << "  EXIT: PREVENT GPF BAD BIND STATE" << std::endl;
                        }
                        (*mp_log) << mp_tab << "zord \"" << mp_key << "\" at <" << o << ">\n";
                    }
                }
            };
            static inspect_t inspect;
            if(mp_log){
                inspect.mp_log = mp_log;
                inspect.mp_tab = "  ";
                inspect.mp_key = curr->first.c_str();
                inspect(curr->second);
            }
        }
        if(mp_log)(*mp_log) << "<GL-END>" << std::endl;
    }
};

#endif // GX_GLX_ABSTRACT_DRAWER_H
