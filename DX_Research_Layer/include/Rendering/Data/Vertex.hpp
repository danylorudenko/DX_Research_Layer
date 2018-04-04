#pragma once

struct Pos
{
    float x, y, z;
};

struct Normal
{
    float x, y, z;
};

struct Tangent
{
    float x, y, z;
};

struct Bitangent
{
    float x, y, z;
};

struct UV
{
    float u, v;
};

struct Vertex
{
    Pos position_;
    Normal normal_;
    Tangent tangent_;
    Bitangent bitangent_;
    UV uv_;
};