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

#ifndef _UPDATE_QUERY_HPP_
#define _UPDATE_QUERY_HPP_

#include <string>

#include <boost/cstdint.hpp>

#include "db/Connection.hpp"
#include "db/Result.hpp"
#include "db/Query.hpp"
#include "db/QueryAssign.hpp"
#include "db/QueryTables.hpp"
#include "db/QueryWhere.hpp"

namespace Database {
class UpdateQuery : public virtual QueryAssign, public virtual QueryTables, public virtual QueryWhere {
public:
    UpdateQuery();
    UpdateQuery(const PConnection connection);

    virtual Result execute();
private:
    UpdateQuery(const UpdateQuery &org);
};
}

#endif // _UPDATE_QUERY_HPP_
