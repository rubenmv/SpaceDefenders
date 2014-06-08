//
// Copyright 2012-2013, Syoyo Fujita.
//
// Licensed under 2-clause BSD liecense.
//
#ifndef _TINY_OBJ_LOADER_H
#define _TINY_OBJ_LOADER_H

#include <string>
#include <vector>
#include <map>

#include "ResourceMesh.h"

namespace tinyobj {

/// Loads .obj from a file.
/// 'shapes' will be filled with parsed shape data
/// The function returns error string.
/// Returns empty string when loading .obj success.
/// 'mtl_basepath' is optional, and used for base path for .mtl file.
std::string LoadObj(
    std::vector<ResourceMesh::shape_t>& shapes,   // [output]
    const char* filename,
    const char* mtl_basepath = NULL);

};

#endif  // _TINY_OBJ_LOADER_H
