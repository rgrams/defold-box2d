#include "shape.h"
#include "utils.h"
#include "extra_utils.h"

namespace box2dDefoldNE {

b2Vec2* parse_vertices(lua_State *L, const char* key,const char* error,int* b2vecSize){
    lua_getfield(L, -1, key);
    if(lua_istable(L,-1)){
        int size = lua_objlen(L,-1);
        b2Vec2* vertices = new b2Vec2[size];
        *b2vecSize = size;
        for (int idx = 1; idx <= size; idx++) {
            lua_pushinteger(L, idx);
            lua_gettable(L, -2);

            //if (lua_isnil(L, -1)){
            //    printf("bread idx:%d\n",idx);
            //    lua_pop(L, 1);
            //    break;
          //  }
            if (lua_isuserdata(L, -1)) {
                if(dmScript::IsVector3(L,  -1)){
                    Vectormath::Aos::Vector3 *value = dmScript::ToVector3(L, -1);
                    b2Vec2 v;
                    v.x = value->getX();
                    v.y = value->getY();
                    vertices[idx-1] = v;
                }else{
                    delete[] vertices;
                    utils::error(L,"vertex not vector3");
                }
            }else{
                delete[] vertices;
                utils::error(L,"vertex not vector3");
            }
            lua_pop(L, 1);
        }
        lua_pop(L,1);
        return vertices;
    }else{
       utils::error(L,error);
    }
    lua_pop(L,1);
}


b2CircleShape* b2CircleShape_from_table(lua_State *L){
    b2CircleShape* shape = new b2CircleShape();
    shape->m_radius = utils::table_get_double_safe(L,"circle_radius","no circle_radius");
    if(utils::table_is_not_nil(L,"circle_position")){
        b2Vec2 position = extra_utils::table_get_b2vec_safe(L,"circle_position","circle_position not vector3");
        shape->m_p.Set(position.x, position.y);
    }
    return shape;
}

b2EdgeShape* b2EdgeShape_from_table(lua_State *L){
    b2EdgeShape* shape = new b2EdgeShape();
    b2Vec2 v1 = extra_utils::table_get_b2vec_safe(L,"edge_v1","edge_v1 not vector3");
    b2Vec2 v2 = extra_utils::table_get_b2vec_safe(L,"edge_v2","edge_v2 not vector3");
    if(utils::table_get_boolean(L,"edge_two_sided",false)){
        shape->SetTwoSided(v1,v2);
    }else{
        b2Vec2 v0 = extra_utils::table_get_b2vec_safe(L,"edge_v0","edge_v0 not vector3");
        b2Vec2 v3 = extra_utils::table_get_b2vec_safe(L,"edge_v3","edge_v3 not vector3");
        shape->SetOneSided(v0,v1,v2,v3);

    }
    return shape;
}

b2PolygonShape* b2PolygonShape_from_table(lua_State *L){
    b2PolygonShape* shape = new b2PolygonShape();
    if(utils::table_get_boolean(L,"box",false)){
        double box_hx = utils::table_get_double_safe(L,"box_hx","no box_hx");
        double box_hy = utils::table_get_double_safe(L,"box_hy","no box_hy");
        bool box_angle_exist = utils::table_is_not_nil(L,"box_angle");
        bool box_center_exist = utils::table_is_not_nil(L,"box_center");
        if(box_angle_exist && box_center_exist){
            double box_angle = utils::table_get_double_safe(L,"box_angle","no box_angle");
            b2Vec2 box_center = extra_utils::table_get_b2vec_safe(L,"box_center","box_center not vector3");
            shape->SetAsBox(box_hx,box_hy,box_center,box_angle);
        }else if(box_angle_exist != box_center_exist){
            if(box_angle_exist){
                utils::error(L,"box_angle exist but box_center not exist");
            }else{
                utils::error(L,"box_center exist but box_angle not exist");
            }
        }else{
            shape->SetAsBox(box_hx,box_hy);
        }
    }else{
        int verticesSize = 0;
        b2Vec2* vertices = parse_vertices(L,"polygon_vertices","no polygon_vertices",&verticesSize);
        shape->Set(vertices,verticesSize);
        delete[] vertices;
    }
    return shape;
}

b2ChainShape* b2ChainShape_from_table(lua_State *L){
    b2ChainShape* shape = new b2ChainShape();
    int verticesSize = 0;
    b2Vec2* vertices = parse_vertices(L,"chain_vertices","no chain_vertices",&verticesSize);
    if(utils::table_get_boolean(L,"chain_loop",false)){
        shape->CreateLoop(vertices,verticesSize);
    }else{
        b2Vec2 vPrev = extra_utils::table_get_b2vec_safe(L,"chain_prev_vertex","chain_prev_vertex not vector3");
        b2Vec2 vNext = extra_utils::table_get_b2vec_safe(L,"chain_next_vertex","chain_next_vertex not vector3");
        shape->CreateChain(vertices,verticesSize,vPrev,vNext);
    }
    delete[] vertices;
    return shape;
}

b2Shape* b2Shape_from_table(lua_State *L, int index){
    if (lua_istable(L, index)) {
        lua_pushvalue(L,index);
        b2Shape* result = NULL;

        b2Shape::Type shapeType = static_cast<b2Shape::Type>(utils::table_get_integer_safe(L,"shape","no shape"));

        if(shapeType == b2Shape::e_circle){
            result = b2CircleShape_from_table(L);
        }else if(shapeType == b2Shape::e_edge){
            result = b2EdgeShape_from_table(L);
        }else if(shapeType == b2Shape::e_polygon){
            result = b2PolygonShape_from_table(L);
        }else if(shapeType == b2Shape::e_chain){
            result = b2ChainShape_from_table(L);
        }



       // b2CircleShape* circle = new ;
       // circle.m_p.Set(2.0f, 3.0f);
       // circle.m_radius = 0.5f;

       lua_pop(L,1);
       return result;
    }else{
        utils::error(L,"b2Shape should be table");
    }

}

}