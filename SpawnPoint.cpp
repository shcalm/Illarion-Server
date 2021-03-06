/*
 * Illarionserver - server for the game Illarion
 * Copyright 2011 Illarion e.V.
 *
 * This file is part of Illarionserver.
 *
 * Illarionserver  is  free  software:  you can redistribute it and/or modify it
 * under the terms of the  GNU  General  Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Illarionserver is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY;  without  even  the  implied  warranty  of  MERCHANTABILITY  or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU  General Public License along with
 * Illarionserver. If not, see <http://www.gnu.org/licenses/>.
 */

#include "SpawnPoint.hpp"

#include <sstream>

#include <boost/cstdint.hpp>

#include "db/SelectQuery.hpp"
#include "db/Result.hpp"

#include "World.hpp"
#include "Random.hpp"
#include "Logger.hpp"

template< typename To, typename From> To stream_convert(const From &from) {
    std::stringstream stream;
    stream << from;
    To to;
    stream >> to;
    return to;
}

//! Creates a new SpawnPoint at <pos>
SpawnPoint::SpawnPoint(const position &pos, int Range, uint16_t Spawnrange, uint16_t Min_Spawntime, uint16_t Max_Spawntime, bool Spawnall) : world(World::get()), spawnpos(pos), range(Range), spawnrange(Spawnrange), min_spawntime(Min_Spawntime), max_spawntime(Max_Spawntime), spawnall(Spawnall) {
    nextspawntime = Random::uniform(min_spawntime, max_spawntime);
    Logger::writeMessage("Spawn","SpawnPoint Konstruktor.");
}

//! Destructor
SpawnPoint::~SpawnPoint() {}

//! add new Monstertyp to SpawnList...
void SpawnPoint::addMonster(const TYPE_OF_CHARACTER_ID &typ, const short int &count) {
    std::list<struct SpawnEntryStruct>::iterator it;

    for (it = SpawnTypes.begin(); it != SpawnTypes.end(); ++it) {
        if ((*it).typ == typ) {
            // monster type already there... just raise max_count
            (*it).max_count+=count;
            return;
        }
    }

    // no entry found... create new one..
    struct SpawnEntryStruct SEtemp;
    SEtemp.typ = typ;
    SEtemp.max_count = count;
    SEtemp.akt_count = 0;
    SpawnTypes.push_back(SEtemp);
}

//! do spawns if possible...
void SpawnPoint::spawn() {
    //std::cout<<"Spawntime for Spawn at pos x="<<spawnpos.x<<" y="<<spawnpos.y<<" z="<<spawnpos.z<<"Spawntime: "<<nextspawntime<<std::endl;
    if (nextspawntime <= 0) {
        //set new spawntime
        nextspawntime = Random::uniform(min_spawntime, max_spawntime);

        //std::cout<<"time to spawn new monster: new spawntime = "<<nextspawntime<<std::endl;
        // do we want monsters to spawn?
        if (configOptions["do_spawn"] == "false") {
            std::cout << "spawning disabled!" << std::endl;
            return;
        }

        std::list<struct SpawnEntryStruct>::iterator it;

        int num;
        Field *tempf;
        Monster *newmonster;

        // check all monstertyps...
        for (it = SpawnTypes.begin(); it != SpawnTypes.end(); ++it) {
            // less monster spawned than we need?
            if ((num=(*it).max_count - (*it).akt_count) > 0) {
                try {
                    // spawn some new baddies :)
                    if (!spawnall) {
                        num = Random::uniform(1, num);
                    }

                    for (int i = 0; i < num; ++i) {
                        position tempPos;
                        //set the new spawnpos in the range of the spawnrange around the spawnpoint
                        tempPos.x = (spawnpos.x - spawnrange) + Random::uniform(0, 2 * spawnrange);
                        tempPos.y = (spawnpos.y - spawnrange) + Random::uniform(0, 2 * spawnrange);
                        tempPos.z = spawnpos.z;

                        //std::cout<<"spawned new monster at pos: "<<tempPos.x<<" "<<tempPos.y<<" "<<tempPos.z<<std::endl;
                        //end of setting the new spawnpos
                        if (world->findEmptyCFieldNear(tempf, tempPos.x, tempPos.y, tempPos.z)) {
                            newmonster = new Monster(it->typ, tempPos, this);
#ifdef SpawnPoint_DEBUG
                            std::cout << "erschaffe Monster " << newmonster->name << " " << tempPos.x << " " << tempPos.y << " " << tempPos.z << std::endl;
#endif
                            (*it).akt_count++;
                            // Monster in die Liste der aktiven Monster einf�gen
                            world->newMonsters.push_back(newmonster);
                            tempf->SetPlayerOnField(true);
                            world->sendCharacterMoveToAllVisiblePlayers(newmonster, NORMALMOVE, 4);
                        } else {
                            std::cout<<"cant find empty field at pos ( "<<tempPos.x<<" "<<tempPos.y<<" "<<tempPos.z<<" )"<<std::endl;
                        }
                    }
                } catch (Monster::unknownIDException &) {
                    std::cerr << "couldn't create monster in SpawnPoint.cpp: " << it->typ << std::endl;
                }
            }
        }
    } else {
        --nextspawntime;
    }
}

void SpawnPoint::dead(const TYPE_OF_CHARACTER_ID &typ) {
    std::list<struct SpawnEntryStruct>::iterator it;

    for (it = SpawnTypes.begin(); it != SpawnTypes.end(); ++it) {
        if ((*it).typ == typ) {
            (*it).akt_count--;
        }
    }
}

bool SpawnPoint::load(const int &id) {
    try {
        Database::SelectQuery query;
        query.addColumn("spawnpoint_monster", "spm_race");
        query.addColumn("spawnpoint_monster", "spm_count");
        query.addEqualCondition("spawnpoint_monster", "spm_id", id);
        query.addServerTable("spawnpoint_monster");

        Database::Result results = query.execute();

        if (!results.empty()) {
            SpawnTypes.clear();

            for (Database::ResultConstIterator itr = results.begin();
                 itr != results.end(); ++itr) {
                addMonster((*itr)["spm_race"].as<TYPE_OF_CHARACTER_ID>(),
                           (*itr)["spm_count"].as<int16_t>());
            }
        }
    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}
