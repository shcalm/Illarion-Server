//  illarionserver - server for the game Illarion
//  Copyright 2011 Illarion e.V.
//
//  This file is part of illarionserver.
//
//  illarionserver is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  illarionserver is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.


#include "WorldMap.hpp"
#include "Map.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp>

WorldMap::WorldMap() {
    lowX = 32767;
    highX = -32768;
}

void WorldMap::clear() {
    maps.clear();
    world_map.clear();

    lowX = 32767;
    highX = -32768;
}

bool WorldMap::mapInRangeOf(short int upperleft_X, short int upperleft_Y, unsigned short int sizex, unsigned short int sizey, short int z) const {
    short int downright_X = upperleft_X + sizex - 1;
    short int downright_Y = upperleft_Y + sizey - 1;

    for (auto it = maps.begin(); it < maps.end(); ++it) {
        if ((*it)->Z_Level == z) {
            if (((*it)->Max_X >= upperleft_X) && ((*it)->Min_X <= downright_X)) {
                if (((*it)->Max_Y >= upperleft_Y) && ((*it)->Min_Y <= downright_Y)) {
                    std::cout << "Map in range at Z:" << (*it)->Z_Level <<
                              " Min_X: " << (*it)->Min_X <<
                              " Max_X: " << (*it)->Max_X <<
                              " Min_Y: " << (*it)->Min_Y <<
                              " Max_Y: " << (*it)->Max_Y <<
                              std::endl;
                    return true;
                }// y
            }// x
        }// z
    }// iterator

    return false;

}



bool WorldMap::findAllMapsInRangeOf(char rnorth, char rsouth, char reast, char rwest, position pos, WorldMap::map_vector_t &ret) const {
    bool found_one = false;
    ret.clear();

    short int upperleft_X = pos.x - rwest;
    short int downright_X = pos.x + reast;

    short int upperleft_Y = pos.y - rnorth;
    short int downright_Y = pos.y + rsouth;

    for (auto it = maps.begin(); it < maps.end(); ++it) {
        if (pos.z == (*it)->Z_Level) {
            if (((*it)->Max_X >= upperleft_X) && ((*it)->Min_X <= downright_X)) {
                if (((*it)->Max_Y >= upperleft_Y) && ((*it)->Min_Y <= downright_Y)) {
                    ret.push_back(*it);
                    found_one = true;
                }// y
            }// x
        }// z
    }// iterator

    return found_one;
}



bool WorldMap::findAllMapsWithXInRangeOf(short int start, short int end, WorldMap::map_vector_t &ret) const {
    bool found_one = false;
    ret.clear();

    for (auto it = maps.begin(); it != maps.end(); ++it) {
        if (((*it)->Max_X >= start) && ((*it)->Min_X <= end)) {
            ret.push_back(*it);
            found_one = true;
        }
    }

    return found_one;
}

bool WorldMap::findMapForPos(short int x, short int y, short int z, WorldMap::map_t &map) const {
    const position p(x, y, z);
    return findMapForPos(p, map);
}

bool WorldMap::findMapForPos(const position &pos, WorldMap::map_t &map) const {
    try {
        map = world_map.at(pos);
        return true;
    } catch (std::out_of_range &e) {
        map.reset();
    }

    return false;
}

bool WorldMap::InsertMap(WorldMap::map_t newMap) {
    if (newMap) {
        for (auto it = maps.begin(); it < maps.end(); ++it) {
            if (*it == newMap) {
                return false;
            }
        }

        if (newMap->Min_X < lowX) {
            lowX = newMap->Min_X;
        }

        if (newMap->Max_X > highX) {
            highX = newMap->Max_X;
        }

        maps.push_back(newMap);

        auto z = newMap->Z_Level;

        for (auto x = newMap->Min_X; x <= newMap->Max_X; ++x) {
            for (auto y = newMap->Min_Y; y <= newMap->Max_Y; ++y) {
                position p(x, y, z);
                world_map[p] = newMap;
            }
        }

        return true;
    }

    return false;

}

void WorldMap::ageContainers() {
    for (auto it = maps.begin(); it != maps.end(); ++it) {
        (*it)->ageContainers();
    }
}

bool WorldMap::exportTo(const std::string &exportDir) const {
    for (auto mapIt = maps.begin(); mapIt != maps.end(); ++mapIt) {
        int16_t minX = (*mapIt)->GetMinX();
        int16_t minY = (*mapIt)->GetMinY();
        // create base filename
        std::string filebase = exportDir + "e_" + boost::lexical_cast<std::string>(minX)
                               + "_" + boost::lexical_cast<std::string>(minY)
                               + "_" + boost::lexical_cast<std::string>((*mapIt)->Z_Level) + ".";
        // export fields file
        std::ofstream fieldsf((filebase + "tiles.txt").c_str());
        // export items file
        std::ofstream itemsf((filebase + "items.txt").c_str());
        // export warps file
        std::ofstream warpsf((filebase + "warps.txt").c_str());

        if (!fieldsf.good() || !itemsf.good() || !warpsf.good()) {
            std::cerr << "could not open output files for item export: " << std::endl;
            std::cerr << filebase << "tiles.txt" << std::endl;
            std::cerr << filebase << "items.txt" << std::endl;
            std::cerr << filebase << "warps.txt" << std::endl;
            return false;
        }

        // export tiles header
        fieldsf << "V: 2" << std::endl;
        fieldsf << "L: " << (*mapIt)->Z_Level << std::endl;
        fieldsf << "X: " << minX << std::endl;
        fieldsf << "Y: " << minY << std::endl;
        fieldsf << "W: " << (*mapIt)->GetWidth() << std::endl;
        fieldsf << "H: " << (*mapIt)->GetHeight() << std::endl;

        // iterate over the map and export...
        short int x, y;

        for (y = minY; y <= (*mapIt)->GetMaxY(); ++y) {
            for (x = minX; x <= (*mapIt)->GetMaxX(); ++x) {
                Field field;

                if ((*mapIt)->GetCFieldAt(field, x, y)) {
                    fieldsf << x-minX << ";" << y-minY << ";" << field.getTileCode() << ";" << field.getMusicId() << std::endl;

                    if (field.IsWarpField()) {
                        position target;
                        field.GetWarpField(target);
                        warpsf << x-minX << ";" << y-minY << ";" << target.x << ";" << target.y << ";" << target.z << std::endl;
                    }

                    ITEMVECTOR itemsv;
                    field.giveExportItems(itemsv);

                    for (auto it = itemsv.cbegin(); it != itemsv.cend(); ++it) {
                        itemsf << x-minX << ";" << y-minY << ";" << it->getId() << ";" << it->getQuality();

                        for (auto dataIt = it->getDataBegin(); dataIt != it->getDataEnd(); ++dataIt) {
                            using boost::algorithm::replace_all;

                            std::string key = dataIt->first;
                            std::string value = dataIt->second;

                            replace_all(key, "\\", "\\\\");
                            replace_all(key, "=", "\\=");
                            replace_all(key, ";", "\\;");
                            replace_all(value, "\\", "\\\\");
                            replace_all(value, "=", "\\=");
                            replace_all(value, ";", "\\;");

                            itemsf << ";" << key << "=" << value;
                        }

                        itemsf << std::endl;
                    }
                }
            }
        }

        fieldsf.close();
        itemsf.close();
        warpsf.close();

    }

    return true;
}

void WorldMap::saveToDisk(const std::string &prefix) const {
    std::ofstream mapinitfile((prefix + "_initmaps").c_str(), std::ios::binary | std::ios::out | std::ios::trunc);

    if (!mapinitfile.good()) {
#ifdef World_DEBUG
        std::cerr << "Could not create initmaps!" << std::endl;
#endif

    } else {
        unsigned short int size = maps.size();
        std::cout << "Saving " << size << " maps." << std::endl;
        mapinitfile.write((char *) & size, sizeof(size));
        char mname[200];

        for (auto mapI = maps.begin(); mapI != maps.end(); ++mapI) {
            mapinitfile.write((char *) & (*mapI)->Z_Level, sizeof((*mapI)->Z_Level));
            mapinitfile.write((char *) & (*mapI)->Min_X, sizeof((*mapI)->Min_X));
            mapinitfile.write((char *) & (*mapI)->Min_Y, sizeof((*mapI)->Min_Y));

            mapinitfile.write((char *) & (*mapI)->Width, sizeof((*mapI)->Width));
            mapinitfile.write((char *) & (*mapI)->Height, sizeof((*mapI)->Height));

            sprintf(mname, "%s_%6d_%6d_%6d", prefix.c_str(), (*mapI)->Z_Level, (*mapI)->Min_X, (*mapI)->Min_Y);
            (*mapI)->Save(mname);
        }

        mapinitfile.close();
    }
}

