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
#ifndef _QUEST_TABLE_HPP_
#define _QUEST_TABLE_HPP_

#include <boost/shared_ptr.hpp>
#include <unordered_map>
#include "data/Table.hpp"
#include "types.hpp"
#include "script/LuaQuestScript.hpp"

class QuestTable: public Table {
public:
    QuestTable();

    inline bool isDataOK() {
        return _dataOK;
    }

    boost::shared_ptr<LuaQuestScript> getQuestScript(TYPE_OF_QUEST_ID id);

private:
    virtual void reload();

    typedef std::unordered_map<TYPE_OF_QUEST_ID, boost::shared_ptr<LuaQuestScript> > QuestMap;
    QuestMap quests;

    bool _dataOK;
};

#endif

