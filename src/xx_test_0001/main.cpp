<<<<<<< HEAD
#include <iostream>

#include <gx_gdom.h>
#include <gx_gdom_json_dump.h>
#include <gx_gdom_print_fn.h>
#include <gx_glm_utils.h>
#include <gx_glx_gdom.h>
#include <gx_glx_abstract_drawer.h>

static gx::ns* root_ns() { static gx::ns single; return &single; }


void codes(std::ostream& os, gx::ns* node)
{
    const char* tabs = "  >";
    gx::ns::code_t::iterator i=node->code.begin(), e=node->code.end();
    for(; i != e; ++i)
    {
        os<<tabs<<i->first<< ":<" <<i->second<<">\n";
        gx::fn_print_info(os, i->second, std::string(tabs)+" |");
    }
}

//declare builders
gx::fn* fn_camera_proj_factory(gx::ns* base, const char* name);
gx::fn* fn_camera_view_factory(gx::ns*, const char* name);
gx::fn* fn_camera_view_proj_factory(gx::ns*, const char* name);

int main(int, char **)
{
    static gx::gdom_json_dump dumps(&std::cout, ""); dumps.tab = "  ";

    // register builders
    gx::fn::builder("camera.proj", fn_camera_proj_factory);
    gx::fn::builder("camera.view", fn_camera_view_factory);
    gx::fn::builder("camera.view_proj", fn_camera_view_proj_factory);

    gx::ns* camera_root = root_ns()->find_ns("camera_0");
    if( NULL == camera_root ){
        camera_root = new gx::ns();
        root_ns()->data["camera_0"] = camera_root;
        camera_root->nest = root_ns();
    }
    gx::fn* camera_proj      = fn_camera_proj_factory      ( camera_root, "camera.proj" );
    gx::fn* camera_view      = fn_camera_view_factory      ( camera_root, "camera.view" );
    gx::fn* camera_view_proj = fn_camera_view_proj_factory ( camera_root, "camera.view_proj" );

    dumps(root_ns()); std::cout << "\n\n";
    codes(std::cout, camera_root); std::cout << "\n\n";

    // ----------
    // Builder of the camera data and code interface is a something like constructor.
    // ----------
    // Test must print results as JSON in STDOUT, so is more useful on read from Python.
    // ----------
    // Each component created ower ns, with self critical gdom - fields, can de reduced
    // to primitive text-based notation builder_name and list of the all input-output
    // edges, except set of the all default edges. Default edges is all edges, those
    // point to component's default dgom fields at serialization pass. Default dgom
    // fields is fields created by builder and place, where point component's edges,
    // when lost self external gdom field path.

    /*
    self: "camera" : interface
        self: "camera.proj" : INTERFACE
          ? f1 far
          ? f1 fov
          ? f1 near
          ? f2 size
          + m4 proj

        self: "camera.view" : INTERFACE
          ? m4 wpos
          + m4 view

        self: "camera.view_proj" : INTERFACE
          ? m4 proj
          ? m4 view
          + m4 view_proj
    */

    /*
    result = gx::fn::create( camera_root, "camera.proj", "proj" );
    if(!result->success()) std::cout << "ERROR:" << result->err().str() << std::endl;
    result->get_ptr()->info(std::cout, "  -"); std::cout<<std::endl;

    result = gx::fn::create( camera_root, "camera.view", "view" );
    if(!result->success()) std::cout << "ERROR:" << result->err().str() << std::endl;
    result->get_ptr()->info(std::cout, "  -"); std::cout<<std::endl;

    result = gx::fn::create( camera_root, "camera.view_proj", "view_proj" );
    if(!result->success()) std::cout << "ERROR:" << result->err().str() << std::endl;
    result->get_ptr()->info(std::cout, "  -"); std::cout<<std::endl;

    // Create functor from gx-gl-aspect family "gx::gl::proj"
    gx::gl::proj p(camera_root);
    camera_root->code["hello"] = &p;  // install in "tests().camera_0"

    // Create functor from gx-gl-aspect family "gx::gl::zord"
    // Create "zord" item in camera;
    gx::ns* z_order = new gx::ns;
    camera_root->set("zord", z_order);                         // z-order's gdom input node
    camera_root->code["zord"] = new gx::gl::zord(camera_root);  // z-order's gx gl code node

    // Порядок действий при создании сцены с камерой:
    //  - Сначала создаётся граф данных GDOM ( RAISE allocation_error );
    //  - Затем в сегменты кода пространств имён добавляются вычислители ( RAISE allocation_error bind_error );
    //  - Полный набор вычислителей не известен заранее, 0..MAX_STD_SIZE;
    //  - Количество вычислителей меняется on-fly, они как добавляются, так и удаляются.
    //  - Если объект GDOM задействован в качестве истичника данных, то его не имеет исходящих  исходящих выходящих
    // Если где-то в ИИ и существует автомат записывающий указатель на gdom объекты
    // в полях шейдер-группы, то он сам отвечает за инвалидацию DG, соотвенственно в
    // данном тесте не рассматривается, хотя как entity узел мог иметь следующий вид:
    //     data:
    //       "?wpos": {"hr":"gx.m4.identity"} => hr.auto
    //       "?shgr": {"hr":"gx.gl.shgr.dirty_red_sphere"} => hr.auto
    //       "?code": {"sm":"hr.auto"}

    // Insert contents in z-ord, is can be anything shgr, proj,
    // adding item "3D-item-0" : [float 16 modelview matrix ]
    //    data:
    //       "wpos" : m4* => gl.view
    //       "shgr" : ns* => gl.view
    //    code:
    //       "gl.view" : gx::gl::view(wpos, shgr) => none  // is read hr-s, signaled on set/del

    // adding shgr modelview matrix we n wpos
    gx::ns* test_ship_0111      = new gx::ns;  // visible item 1
    gx::ns* test_ship_0112      = new gx::ns;  // visible item 2
    gx::ns* test_ship_0113      = new gx::ns;  // visible item 3

    gx::m4* test_ship_0111_wpos = new gx::m4;  // first visible item world position
    gx::m4* test_ship_0112_wpos = new gx::m4;  // second visible item world position
    gx::m4* test_ship_0113_wpos = new gx::m4;  // third visible item world position

    gx::ns* test_ship_shgr      = new gx::ns;  // shared shading group
    gx::ns* test_ship_mesh      = new gx::ns;  // shared mesh attributes gxa
    gx::ns* test_ship_mtrl      = new gx::ns;  // shared material prog, vars, samplers

    test_ship_shgr->set( "mesh", test_ship_mesh );
    test_ship_shgr->set( "mtrl", test_ship_mtrl );

    z_order->set( "test_ship_0111", test_ship_0111 );
       test_ship_0111->set( "wpos", test_ship_0111_wpos );
       test_ship_0111->set( "shgr", test_ship_shgr );

    z_order->set( "test_ship_0112", test_ship_0112 );
       test_ship_0112->set( "wpos", test_ship_0112_wpos );
       test_ship_0112->set( "shgr", test_ship_shgr );

    z_order->set( "test_ship_0113", test_ship_0113 );
       test_ship_0113->set( "wpos", test_ship_0113_wpos );
       test_ship_0113->set( "shgr", test_ship_shgr );

    z_order->set( "from", new gx::ns );
    z_order->set( "big_bodun", new gx::ns );


    // now we can inspect ns-code-functors as with gx::gl::proc
    abstract_drawer drawer(camera_root, &std::cout);
    drawer.paint();

    gx::f1* test_f1 = new gx::f1;

    gx::f1* alias2 = NULL;
    test_f1->at(0) = 3.24f;
    std::cout << test_f1->at(0) << std::endl;

    gx::edge test_edge( NULL, &alias2, "" );
    test_edge.mp_gdom = test_f1;
    test_edge.connect(); // => return success;
    test_edge.success ? std::cout << "alias2 => " << alias2->at(0) << std::endl : std::cout << "alias2 => ??" << std::endl;

    // edge(NULL,base,""

    // Each camera must containt self own Z-order of objects to paint, but de-facto is gx-xx-ns object,
    // sync or async managed by AI as non-gl trivial GDOM-ns. Some special gx-gl-aspect-draw-order not needed!
    // gx-gl-proc.on(xx).on(ns)
    // xx-not-some-ns
    // gl-proc-fn at first visit to scene-ns-item.
    //     must have "wpos" attribute "m4" - if has, item "code" [] item "scene" - new test ns, each item can have wpos

    // Each camera must containt self own Z-order of objects to paint.
    // Z-order are functor with pointer to scene, where each object have:
    //
    //   get "scene"  // where all valuable items have "wpos" - world transform )
    //                // each object in scene must send signal set/del to each Z-order;
    //                // each "scene"'s add/del must send signal to each Z-order;
    //
    //   set "z-ord"  // output list with Z-order );
    //   get "wpos"   // own camera eye pos;
    //     -nope-     // if len(camera_eye_pos - object_pos) < small_flost_delta no signal needed;
*/
    return 1;

/*
    // creating gx::sm State Machine from Zero.
    //gx::sm gxsm;  // empty gx::dgom-side state machine's interface
    gx::u0 gxu0("Mababumba Sample");  // data field
    //gxu0.c_sm = &gxsm;                // put machine in path

    void gx_dg_reactor_factory_example( gx::xx* );  // declare reactor's factory

    typedef void ( *gx_dg_reactor_factory_t ) ( gx::xx* );  // reactor factory type
    static std::map<std::string, gx_dg_reactor_factory_t> dg_reactors_factory;
    dg_reactors_factory["gx_dg_reactor_factory_example"] = gx_dg_reactor_factory_example;

    // it is can be cheked like:
    dg_reactors_factory["gx_dg_reactor_factory_example"](&gxu0);

    // Why factory? If reactor return some static instance of reactor - this way
    // is only singleton pattern support. If factory return different pointer to
    // each gx::xx instance, this way work like design pattern "Virtual Constructor"
    // Name of "dg_reactor" must be defined in factory, is must reduce "naming chaos".
    // Как читать данные из gDOM: В принципе положенно проверять покраску DG и только
    // если данные считаются устаревшими вызывать функцию перерасчета, если успех, то
    // красить данные в пригодные для использования. Вопрос: что делать если функция
    // пересчета выбросила Exception. Или другим способом оставила данные в состоянии
    // инвалид. Для системы реального времени это критический вопрос.
    // Где-то в функторе вызывающем gx::xx::get() может разместиться ИИ анализирующее
    // подобную ситуацию: флаг инвалид, вызываю calc, флаг всё едино инвалид. Это
    // не даёт анализирующему механизму никаких данных о причине такого явления. Выброс
    // исключения даст данные о том что произошло, но даст понимание где именно это
    // случилось только при условии предварительного трассирования - а это дикие накладные
    // расходы для всех "штатных" режимов работы. Меньше всего в системе реального
    // времени хочется терять быстродействие при "штатных" режимах работы. Значит
    // предварительная трассировка отпадает сразу. И тривиальный набор флагов покраски
    // DG становится недостаточным. Введём несколько новых:
    // state = GOOD - 0 Данные посчитаны и их можно забрать 0
    // state = CALC - 1 Данные нужно перерассчитать вызвав метод calc() узла
    // FAIL - 2 Автомат калькулятора не обновил данные: Это не обязательно ошибка делающая
    //          дальнейшую работу приложения невозможным. Просто флаг устанавливаемый со стороны
    //          калькулятора и говорящий о том что возникла нештатная ситуация. Далеко не каждый
    //          узел gDOM снабжен автоматом калькулятора, но устанавливает и сбрасывает флаг FAIL
    //          именно он. Базовый интерфейс калькулятора снабжается:
    //           -- ostream* выключаемым логом,
    //           -- ostream* логом ошибки,
    //           -- полем флагов состяния INIT, BIND, STEP, WAIT, FAIL
    //           -- Флагом, позволяющим понять, содержит ли текущие значение результат предыдущего рассчета
    // Обнаружив после вызова CALC флаг FAIL функтор запросивший значение может обратиться к интерфейсу
    // калькулятора для анализа его флагов, и только обнаружив флаг FAIL приступить к анализу ошибки.

    // Чем "нативный" калькулятор отличается от гостевых из std::map<std::string, gx::fn*> gx::xx::code ?
    // Нативный расположен отдельно от остальных и занят исключительно полем данных. Сигнал о пересчете
    // позволяет каждому гостевому калькулятору перейдти в состояние пересчета аспекто-зависимых представлений
    // о данных, с калькуляцией данных нативным автоматом. Если какой-то другой аспект уже исполнил валидацию
    // данных вызвав нативный калькулятор, то гостевые генераторы представлений лишатся знаний о том, что данные
    // поменялись, и будут считать свои представления о данных валидными. Таким образом уведомление всех
    // элементов словаря "code" о покраске в CALC более чем оправдано и не более чем покраска в CALC
    // представлений через генераторы этих самых представлений. Инвалидация представлений может оказаться отложенной
    // и произведенной в момент готовности определённого устройства связанного с гостевым аспектом. Почему не
    // вынести все представления прямо в DOM дерево и не прописать явное ребро DG? Причина проста, представления
    // на стороне видео карты, аппаратного физического акселератора или базы данных, недоступны для такого подхода.
    // И могут быть изменены только при наступлении определённых условий реального времени.

    dg_bind_calc( gx::xx::tests()->at("camera_0")->try_ns(), "proj", "gx.glm.proj", {"fov", "size"} );

    gx::path test_path("camera_0.proj");
    std::cout<<test_path.info()<<std::endl<<std::endl;
    std::cout<<"\""<<test_path.repr()<<"\""<<std::endl;
    gx::xx* temp_xx = test_path.find(gx::xx::tests());
    static gx::xx::type_name get_type_name;
    if (temp_xx)
        std::cout<<"test_path.find => "<< get_type_name(temp_xx) <<" at <"<<temp_xx<<">"<<std::endl;
    else
        std::cout<<"test_path.find => "<< test_path.err.str() <<std::endl;
    std::cout<<test_path.subpath(1)<<std::endl;


    return 1;

    std::string input_str;
    while (input_str!="exit"){
      std::cout<<">>";
      std::getline(std::cin, input_str);
      std::cout<<"username = "<<input_str<<std::endl;
    }

    // для проверки уведомлений о SET DEL: Надо послать в поле DOM SET,
    // и прочитать состояния всех нативных и гостевых аспектов в DG --
    // до выполнения каких либо валидационных действий все состояния должны
    // быть CALC. Так-же протестировать DEL -- состояния должны быть BIND

    return 1; //return a.exec();
    */
}

/*
void gx_dg_reactor_factory_example(gx::xx* p_xx)
{
    // p_xx here, is a concrete exists gdom node, "time of the life in memory"
    // of this node are equal or more then time_life of the created reactor instance.

    struct reactor_t: gx::fn {
                                // public state mean has self own out/err/fail-state and try-catch on step()

        gx::m4* mp_model_view;  // some position/rotation at global()["scene"]["objects"]["camera"]
                                // where attached sm's write actual position/rotation
        gx::m4* mp_projection;  // some projection in global()["scene"]["objects"]["camera3"]
                                // where attached sm's write projection. Note - projection depends
                                // multiple incoming values:
        gx::f1 *mp_fov, *mp_near, *mp_far;
        gx::f2 *mp_size;
                                //   lookat:f3,
                                //       up:f3
                                // SET each of them must change attached sm state to "calc" from "good".
                                // PROBLEM HERE: SET UNKNOWN MACHINE TO UNKNOWN "state-instance" use only
                                // name or predefined interface. Any state of primary machine can do some
                                // check on field used only for this "paint in invalid", so "good" is a
                                // member of each gx::xx, include gx::sm. "good" initially are 0.
                                // "good" switch to 0 on install dependendence edge in DG => each field
                                // must contain std::list<gx::xx*> with depends, but can't known all
                                // depended nodes. For example some "fov" field can be used as source
                                // for multiple matrices as fov, and as drawable text-label source from
                                // multiple debug windows.
        reactor_t(gx::xx*) {
            // IS A VERY SLOW CALC-STATE all exceptions must be stored right here in std::ostringstream err
            // BIND: reset all temporary data and reinit all edges
            // gx::set_u0(&mp_self_u0, p_xx);  // is raise syntax error or init 'mp_self_u0';
            gx::set_m4(&mp_model_view, gx::xx::tests() -> at("m4_as_model_view"));
            gx::set_m4(&mp_projection, gx::xx::tests() -> at("m4_as_projection"));
            std::cout<<"bind>camera_0.fov"<<std::endl;
            gx::set_f1(&mp_fov  ,gx::xx::tests()->at("camera_0")->try_ns()->at("fov" ));

            bind(&mp_fov, gx::xx::tests(), "camera_0.fov" );

            std::cout<<"bind>camera_0.size"<<std::endl;
            gx::set_f2(&mp_size ,gx::xx::tests()->at("camera_0")->try_ns()->at("size"));
            std::cout<<"bind>camera_0.near"<<std::endl;
            gx::set_f1(&mp_near ,gx::xx::tests()->at("camera_0")->try_ns()->at("near"));
            std::cout<<"bind>camera_0.far"<<std::endl;
            gx::set_f1(&mp_far  ,gx::xx::tests()->at("camera_0")->try_ns()->at("far" ));
            // CALC, before switch to calc state check self input nodes flags,
            // add self instance to dg set. Is guarantee paint-to-calc on change input value
        }
        glm::mat4 GetProjectionMatrix() const
        {
            // need separate "proj" field in camera, that depend
            //   "size", "fov", "near", "far"
            float aspect = mp_size->at(0) / (mp_size->at(1) > 1) ? mp_size->at(1) : 1;
            return glm::perspective
                   ( mp_fov->at(0)    // 45.f   <= fov
                   , aspect           // width/height
                   , mp_near->at(0)   // 0.1f   <= near clip plane
                   , mp_far->at(0)    // 100.0f <= far clip plane
                   );
        }
        glm::mat4 GetViewMatrix() const
        {
            // need separate "view" field in camera, that depend only "wpos"
            glm::mat4 camera_world_position(1.f);                 // camera world position in scene
            float* cwp = glm::value_ptr(camera_world_position);   // used as float[16]
            glm::vec3 camera_position(cwp[12], cwp[13], cwp[14]); // cwp translation;
            glm::vec3 z(cwp[ 2], cwp[ 6], cwp[10]); // cwp z-vector;
            glm::vec3 look_at_point = camera_position + z;
            glm::vec3 up_vector      (cwp[ 1], cwp[ 5], cwp[ 9]); // cwp y-vector;
            return glm::lookAt(camera_position, look_at_point, up_vector);
        }
        glm::mat4 GetViewProjectionMatrix() const
        {
            // need separate "view_proj" field in camera, that depend "proj" and "view"
            // drawer object must get-stored-dom-object-with-invalidation at first camera
            // paint in each frame, and use glm::mat4 stack copy to calculate MVP for each
            // object drawed by this camera
            return GetProjectionMatrix() * GetViewMatrix();  //to calc MVP * GetModelMatrix
        }
        virtual void on(gx::fn::proc* p){p->on(this);}
        virtual void step(){ std::cout << "init_u0_state<" << this << ">.step()" << std::endl; }
    };
    std::cout << "blah\nblah\n\n";
    try {
        reactor_t* p_reactor = new reactor_t(p_xx);
        p_xx->code["gx_dg_reactor_factory_example"] = p_reactor;
    }
    catch( const gx::type_guard::unexpected& e ) {
        std::cout << "gx::type_guard on create instance" << e.what() << std::endl;
    }
    catch( const std::out_of_range& e) {
        std::cout << "std::out_of_range" << e.what() << std::endl;
    }
    catch( const std::bad_alloc& e ) {
        std::cout << "std::bad_alloc&" << e.what() << std::endl;
    }
}
*/
=======
#include <iostream>

#include <gx_gdom.h>
#include <gx_gdom_json_dump.h>
#include <gx_gdom_print_fn.h>
#include <gx_glm_utils.h>
#include <gx_glx_gdom.h>
#include <gx_glx_abstract_drawer.h>

static gx::ns* root_ns() { static gx::ns single; return &single; }


void codes(std::ostream& os, gx::ns* node)
{
    const char* tabs = "  >";
    gx::ns::code_t::iterator i=node->code.begin(), e=node->code.end();
    for(; i != e; ++i)
    {
        os<<tabs<<i->first<< ":<" <<i->second<<">\n";
        gx::fn_print_info(os, i->second, std::string(tabs)+" |");
    }
}

//declare builders
gx::fn* fn_camera_proj_factory(gx::ns* base, const char* name);
gx::fn* fn_camera_view_factory(gx::ns*, const char* name);
gx::fn* fn_camera_view_proj_factory(gx::ns*, const char* name);

int main(int, char **)
{
    static gx::gdom_json_dump dumps(&std::cout, ""); dumps.tab = "  ";

    // register builders
    gx::fn::builder("camera.proj", fn_camera_proj_factory);
    gx::fn::builder("camera.view", fn_camera_view_factory);
    gx::fn::builder("camera.view_proj", fn_camera_view_proj_factory);

    gx::ns* camera_root = root_ns()->find_ns("camera_0");
    if( NULL == camera_root ){
        camera_root = new gx::ns();
        root_ns()->data["camera_0"] = camera_root;
        camera_root->nest = root_ns();
    }
    gx::fn* camera_proj      = fn_camera_proj_factory      ( camera_root, "camera.proj" );
    gx::fn* camera_view      = fn_camera_view_factory      ( camera_root, "camera.view" );
    gx::fn* camera_view_proj = fn_camera_view_proj_factory ( camera_root, "camera.view_proj" );

    dumps(root_ns()); std::cout << "\n\n";
    codes(std::cout, camera_root); std::cout << "\n\n";

    // ----------
    // Builder of the camera data and code interface is a something like constructor.
    // ----------
    // Test must print results as JSON in STDOUT, so is more useful on read from Python.
    // ----------
    // Each component created ower ns, with self critical gdom - fields, can de reduced
    // to primitive text-based notation builder_name and list of the all input-output
    // edges, except set of the all default edges. Default edges is all edges, those
    // point to component's default dgom fields at serialization pass. Default dgom
    // fields is fields created by builder and place, where point component's edges,
    // when lost self external gdom field path.

    /*
    self: "camera" : interface
        self: "camera.proj" : INTERFACE
          ? f1 far
          ? f1 fov
          ? f1 near
          ? f2 size
          + m4 proj

        self: "camera.view" : INTERFACE
          ? m4 wpos
          + m4 view

        self: "camera.view_proj" : INTERFACE
          ? m4 proj
          ? m4 view
          + m4 view_proj
    */

    /*
    result = gx::fn::create( camera_root, "camera.proj", "proj" );
    if(!result->success()) std::cout << "ERROR:" << result->err().str() << std::endl;
    result->get_ptr()->info(std::cout, "  -"); std::cout<<std::endl;

    result = gx::fn::create( camera_root, "camera.view", "view" );
    if(!result->success()) std::cout << "ERROR:" << result->err().str() << std::endl;
    result->get_ptr()->info(std::cout, "  -"); std::cout<<std::endl;

    result = gx::fn::create( camera_root, "camera.view_proj", "view_proj" );
    if(!result->success()) std::cout << "ERROR:" << result->err().str() << std::endl;
    result->get_ptr()->info(std::cout, "  -"); std::cout<<std::endl;

    // Create functor from gx-gl-aspect family "gx::gl::proj"
    gx::gl::proj p(camera_root);
    camera_root->code["hello"] = &p;  // install in "tests().camera_0"

    // Create functor from gx-gl-aspect family "gx::gl::zord"
    // Create "zord" item in camera;
    gx::ns* z_order = new gx::ns;
    camera_root->set("zord", z_order);                         // z-order's gdom input node
    camera_root->code["zord"] = new gx::gl::zord(camera_root);  // z-order's gx gl code node

    // Порядок действий при создании сцены с камерой:
    //  - Сначала создаётся граф данных GDOM ( RAISE allocation_error );
    //  - Затем в сегменты кода пространств имён добавляются вычислители ( RAISE allocation_error bind_error );
    //  - Полный набор вычислителей не известен заранее, 0..MAX_STD_SIZE;
    //  - Количество вычислителей меняется on-fly, они как добавляются, так и удаляются.
    //  - Если объект GDOM задействован в качестве истичника данных, то его не имеет исходящих  исходящих выходящих
    // Если где-то в ИИ и существует автомат записывающий указатель на gdom объекты
    // в полях шейдер-группы, то он сам отвечает за инвалидацию DG, соотвенственно в
    // данном тесте не рассматривается, хотя как entity узел мог иметь следующий вид:
    //     data:
    //       "?wpos": {"hr":"gx.m4.identity"} => hr.auto
    //       "?shgr": {"hr":"gx.gl.shgr.dirty_red_sphere"} => hr.auto
    //       "?code": {"sm":"hr.auto"}

    // Insert contents in z-ord, is can be anything shgr, proj,
    // adding item "3D-item-0" : [float 16 modelview matrix ]
    //    data:
    //       "wpos" : m4* => gl.view
    //       "shgr" : ns* => gl.view
    //    code:
    //       "gl.view" : gx::gl::view(wpos, shgr) => none  // is read hr-s, signaled on set/del

    // adding shgr modelview matrix we n wpos
    gx::ns* test_ship_0111      = new gx::ns;  // visible item 1
    gx::ns* test_ship_0112      = new gx::ns;  // visible item 2
    gx::ns* test_ship_0113      = new gx::ns;  // visible item 3

    gx::m4* test_ship_0111_wpos = new gx::m4;  // first visible item world position
    gx::m4* test_ship_0112_wpos = new gx::m4;  // second visible item world position
    gx::m4* test_ship_0113_wpos = new gx::m4;  // third visible item world position

    gx::ns* test_ship_shgr      = new gx::ns;  // shared shading group
    gx::ns* test_ship_mesh      = new gx::ns;  // shared mesh attributes gxa
    gx::ns* test_ship_mtrl      = new gx::ns;  // shared material prog, vars, samplers

    test_ship_shgr->set( "mesh", test_ship_mesh );
    test_ship_shgr->set( "mtrl", test_ship_mtrl );

    z_order->set( "test_ship_0111", test_ship_0111 );
       test_ship_0111->set( "wpos", test_ship_0111_wpos );
       test_ship_0111->set( "shgr", test_ship_shgr );

    z_order->set( "test_ship_0112", test_ship_0112 );
       test_ship_0112->set( "wpos", test_ship_0112_wpos );
       test_ship_0112->set( "shgr", test_ship_shgr );

    z_order->set( "test_ship_0113", test_ship_0113 );
       test_ship_0113->set( "wpos", test_ship_0113_wpos );
       test_ship_0113->set( "shgr", test_ship_shgr );

    z_order->set( "from", new gx::ns );
    z_order->set( "big_bodun", new gx::ns );


    // now we can inspect ns-code-functors as with gx::gl::proc
    abstract_drawer drawer(camera_root, &std::cout);
    drawer.paint();

    gx::f1* test_f1 = new gx::f1;

    gx::f1* alias2 = NULL;
    test_f1->at(0) = 3.24f;
    std::cout << test_f1->at(0) << std::endl;

    gx::edge test_edge( NULL, &alias2, "" );
    test_edge.mp_gdom = test_f1;
    test_edge.connect(); // => return success;
    test_edge.success ? std::cout << "alias2 => " << alias2->at(0) << std::endl : std::cout << "alias2 => ??" << std::endl;

    // edge(NULL,base,""

    // Each camera must containt self own Z-order of objects to paint, but de-facto is gx-xx-ns object,
    // sync or async managed by AI as non-gl trivial GDOM-ns. Some special gx-gl-aspect-draw-order not needed!
    // gx-gl-proc.on(xx).on(ns)
    // xx-not-some-ns
    // gl-proc-fn at first visit to scene-ns-item.
    //     must have "wpos" attribute "m4" - if has, item "code" [] item "scene" - new test ns, each item can have wpos

    // Each camera must containt self own Z-order of objects to paint.
    // Z-order are functor with pointer to scene, where each object have:
    //
    //   get "scene"  // where all valuable items have "wpos" - world transform )
    //                // each object in scene must send signal set/del to each Z-order;
    //                // each "scene"'s add/del must send signal to each Z-order;
    //
    //   set "z-ord"  // output list with Z-order );
    //   get "wpos"   // own camera eye pos;
    //     -nope-     // if len(camera_eye_pos - object_pos) < small_flost_delta no signal needed;
*/
    return 1;

/*
    // creating gx::sm State Machine from Zero.
    //gx::sm gxsm;  // empty gx::dgom-side state machine's interface
    gx::u0 gxu0("Mababumba Sample");  // data field
    //gxu0.c_sm = &gxsm;                // put machine in path

    void gx_dg_reactor_factory_example( gx::xx* );  // declare reactor's factory

    typedef void ( *gx_dg_reactor_factory_t ) ( gx::xx* );  // reactor factory type
    static std::map<std::string, gx_dg_reactor_factory_t> dg_reactors_factory;
    dg_reactors_factory["gx_dg_reactor_factory_example"] = gx_dg_reactor_factory_example;

    // it is can be cheked like:
    dg_reactors_factory["gx_dg_reactor_factory_example"](&gxu0);

    // Why factory? If reactor return some static instance of reactor - this way
    // is only singleton pattern support. If factory return different pointer to
    // each gx::xx instance, this way work like design pattern "Virtual Constructor"
    // Name of "dg_reactor" must be defined in factory, is must reduce "naming chaos".
    // Как читать данные из gDOM: В принципе положенно проверять покраску DG и только
    // если данные считаются устаревшими вызывать функцию перерасчета, если успех, то
    // красить данные в пригодные для использования. Вопрос: что делать если функция
    // пересчета выбросила Exception. Или другим способом оставила данные в состоянии
    // инвалид. Для системы реального времени это критический вопрос.
    // Где-то в функторе вызывающем gx::xx::get() может разместиться ИИ анализирующее
    // подобную ситуацию: флаг инвалид, вызываю calc, флаг всё едино инвалид. Это
    // не даёт анализирующему механизму никаких данных о причине такого явления. Выброс
    // исключения даст данные о том что произошло, но даст понимание где именно это
    // случилось только при условии предварительного трассирования - а это дикие накладные
    // расходы для всех "штатных" режимов работы. Меньше всего в системе реального
    // времени хочется терять быстродействие при "штатных" режимах работы. Значит
    // предварительная трассировка отпадает сразу. И тривиальный набор флагов покраски
    // DG становится недостаточным. Введём несколько новых:
    // state = GOOD - 0 Данные посчитаны и их можно забрать 0
    // state = CALC - 1 Данные нужно перерассчитать вызвав метод calc() узла
    // FAIL - 2 Автомат калькулятора не обновил данные: Это не обязательно ошибка делающая
    //          дальнейшую работу приложения невозможным. Просто флаг устанавливаемый со стороны
    //          калькулятора и говорящий о том что возникла нештатная ситуация. Далеко не каждый
    //          узел gDOM снабжен автоматом калькулятора, но устанавливает и сбрасывает флаг FAIL
    //          именно он. Базовый интерфейс калькулятора снабжается:
    //           -- ostream* выключаемым логом,
    //           -- ostream* логом ошибки,
    //           -- полем флагов состяния INIT, BIND, STEP, WAIT, FAIL
    //           -- Флагом, позволяющим понять, содержит ли текущие значение результат предыдущего рассчета
    // Обнаружив после вызова CALC флаг FAIL функтор запросивший значение может обратиться к интерфейсу
    // калькулятора для анализа его флагов, и только обнаружив флаг FAIL приступить к анализу ошибки.

    // Чем "нативный" калькулятор отличается от гостевых из std::map<std::string, gx::fn*> gx::xx::code ?
    // Нативный расположен отдельно от остальных и занят исключительно полем данных. Сигнал о пересчете
    // позволяет каждому гостевому калькулятору перейдти в состояние пересчета аспекто-зависимых представлений
    // о данных, с калькуляцией данных нативным автоматом. Если какой-то другой аспект уже исполнил валидацию
    // данных вызвав нативный калькулятор, то гостевые генераторы представлений лишатся знаний о том, что данные
    // поменялись, и будут считать свои представления о данных валидными. Таким образом уведомление всех
    // элементов словаря "code" о покраске в CALC более чем оправдано и не более чем покраска в CALC
    // представлений через генераторы этих самых представлений. Инвалидация представлений может оказаться отложенной
    // и произведенной в момент готовности определённого устройства связанного с гостевым аспектом. Почему не
    // вынести все представления прямо в DOM дерево и не прописать явное ребро DG? Причина проста, представления
    // на стороне видео карты, аппаратного физического акселератора или базы данных, недоступны для такого подхода.
    // И могут быть изменены только при наступлении определённых условий реального времени.

    dg_bind_calc( gx::xx::tests()->at("camera_0")->try_ns(), "proj", "gx.glm.proj", {"fov", "size"} );

    gx::path test_path("camera_0.proj");
    std::cout<<test_path.info()<<std::endl<<std::endl;
    std::cout<<"\""<<test_path.repr()<<"\""<<std::endl;
    gx::xx* temp_xx = test_path.find(gx::xx::tests());
    static gx::xx::type_name get_type_name;
    if (temp_xx)
        std::cout<<"test_path.find => "<< get_type_name(temp_xx) <<" at <"<<temp_xx<<">"<<std::endl;
    else
        std::cout<<"test_path.find => "<< test_path.err.str() <<std::endl;
    std::cout<<test_path.subpath(1)<<std::endl;


    return 1;

    std::string input_str;
    while (input_str!="exit"){
      std::cout<<">>";
      std::getline(std::cin, input_str);
      std::cout<<"username = "<<input_str<<std::endl;
    }

    // для проверки уведомлений о SET DEL: Надо послать в поле DOM SET,
    // и прочитать состояния всех нативных и гостевых аспектов в DG --
    // до выполнения каких либо валидационных действий все состояния должны
    // быть CALC. Так-же протестировать DEL -- состояния должны быть BIND

    return 1; //return a.exec();
    */
}

/*
void gx_dg_reactor_factory_example(gx::xx* p_xx)
{
    // p_xx here, is a concrete exists gdom node, "time of the life in memory"
    // of this node are equal or more then time_life of the created reactor instance.

    struct reactor_t: gx::fn {
                                // public state mean has self own out/err/fail-state and try-catch on step()

        gx::m4* mp_model_view;  // some position/rotation at global()["scene"]["objects"]["camera"]
                                // where attached sm's write actual position/rotation
        gx::m4* mp_projection;  // some projection in global()["scene"]["objects"]["camera3"]
                                // where attached sm's write projection. Note - projection depends
                                // multiple incoming values:
        gx::f1 *mp_fov, *mp_near, *mp_far;
        gx::f2 *mp_size;
                                //   lookat:f3,
                                //       up:f3
                                // SET each of them must change attached sm state to "calc" from "good".
                                // PROBLEM HERE: SET UNKNOWN MACHINE TO UNKNOWN "state-instance" use only
                                // name or predefined interface. Any state of primary machine can do some
                                // check on field used only for this "paint in invalid", so "good" is a
                                // member of each gx::xx, include gx::sm. "good" initially are 0.
                                // "good" switch to 0 on install dependendence edge in DG => each field
                                // must contain std::list<gx::xx*> with depends, but can't known all
                                // depended nodes. For example some "fov" field can be used as source
                                // for multiple matrices as fov, and as drawable text-label source from
                                // multiple debug windows.
        reactor_t(gx::xx*) {
            // IS A VERY SLOW CALC-STATE all exceptions must be stored right here in std::ostringstream err
            // BIND: reset all temporary data and reinit all edges
            // gx::set_u0(&mp_self_u0, p_xx);  // is raise syntax error or init 'mp_self_u0';
            gx::set_m4(&mp_model_view, gx::xx::tests() -> at("m4_as_model_view"));
            gx::set_m4(&mp_projection, gx::xx::tests() -> at("m4_as_projection"));
            std::cout<<"bind>camera_0.fov"<<std::endl;
            gx::set_f1(&mp_fov  ,gx::xx::tests()->at("camera_0")->try_ns()->at("fov" ));

            bind(&mp_fov, gx::xx::tests(), "camera_0.fov" );

            std::cout<<"bind>camera_0.size"<<std::endl;
            gx::set_f2(&mp_size ,gx::xx::tests()->at("camera_0")->try_ns()->at("size"));
            std::cout<<"bind>camera_0.near"<<std::endl;
            gx::set_f1(&mp_near ,gx::xx::tests()->at("camera_0")->try_ns()->at("near"));
            std::cout<<"bind>camera_0.far"<<std::endl;
            gx::set_f1(&mp_far  ,gx::xx::tests()->at("camera_0")->try_ns()->at("far" ));
            // CALC, before switch to calc state check self input nodes flags,
            // add self instance to dg set. Is guarantee paint-to-calc on change input value
        }
        glm::mat4 GetProjectionMatrix() const
        {
            // need separate "proj" field in camera, that depend
            //   "size", "fov", "near", "far"
            float aspect = mp_size->at(0) / (mp_size->at(1) > 1) ? mp_size->at(1) : 1;
            return glm::perspective
                   ( mp_fov->at(0)    // 45.f   <= fov
                   , aspect           // width/height
                   , mp_near->at(0)   // 0.1f   <= near clip plane
                   , mp_far->at(0)    // 100.0f <= far clip plane
                   );
        }
        glm::mat4 GetViewMatrix() const
        {
            // need separate "view" field in camera, that depend only "wpos"
            glm::mat4 camera_world_position(1.f);                 // camera world position in scene
            float* cwp = glm::value_ptr(camera_world_position);   // used as float[16]
            glm::vec3 camera_position(cwp[12], cwp[13], cwp[14]); // cwp translation;
            glm::vec3 z(cwp[ 2], cwp[ 6], cwp[10]); // cwp z-vector;
            glm::vec3 look_at_point = camera_position + z;
            glm::vec3 up_vector      (cwp[ 1], cwp[ 5], cwp[ 9]); // cwp y-vector;
            return glm::lookAt(camera_position, look_at_point, up_vector);
        }
        glm::mat4 GetViewProjectionMatrix() const
        {
            // need separate "view_proj" field in camera, that depend "proj" and "view"
            // drawer object must get-stored-dom-object-with-invalidation at first camera
            // paint in each frame, and use glm::mat4 stack copy to calculate MVP for each
            // object drawed by this camera
            return GetProjectionMatrix() * GetViewMatrix();  //to calc MVP * GetModelMatrix
        }
        virtual void on(gx::fn::proc* p){p->on(this);}
        virtual void step(){ std::cout << "init_u0_state<" << this << ">.step()" << std::endl; }
    };
    std::cout << "blah\nblah\n\n";
    try {
        reactor_t* p_reactor = new reactor_t(p_xx);
        p_xx->code["gx_dg_reactor_factory_example"] = p_reactor;
    }
    catch( const gx::type_guard::unexpected& e ) {
        std::cout << "gx::type_guard on create instance" << e.what() << std::endl;
    }
    catch( const std::out_of_range& e) {
        std::cout << "std::out_of_range" << e.what() << std::endl;
    }
    catch( const std::bad_alloc& e ) {
        std::cout << "std::bad_alloc&" << e.what() << std::endl;
    }
}
*/
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
