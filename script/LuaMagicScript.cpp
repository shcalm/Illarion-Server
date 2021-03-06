/*
 *  illarionserver - server for the game Illarion
 *  Copyright 2011 Illarion e.V.
 *
 *  This file is part of illarionserver.
 *
 *  illarionserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  illarionserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LuaMagicScript.hpp"
#include "Character.hpp"
#include "Item.hpp"
#include "fuse_ptr.hpp"

LuaMagicScript::LuaMagicScript(std::string filename, unsigned long int MagicFlag) throw(ScriptException)
    : LuaScript(filename), _MagicFlag(MagicFlag) {
    init_functions();
}

LuaMagicScript::~LuaMagicScript() throw() {}

void LuaMagicScript::init_functions() {
    luabind::object globals = luabind::globals(_luaState);
    globals["thisSpell"] = _MagicFlag;
}

void LuaMagicScript::CastMagic(Character *caster, unsigned char ltastate) {
    fuse_ptr<Character> fuse_caster(caster);
    callEntrypoint("CastMagic", fuse_caster, ltastate);
}

void LuaMagicScript::CastMagicOnCharacter(Character *caster, Character *target, unsigned char ltastate) {
    fuse_ptr<Character> fuse_caster(caster);
    fuse_ptr<Character> fuse_target(target);
    callEntrypoint("CastMagicOnCharacter", fuse_caster, fuse_target, ltastate);
}

void LuaMagicScript::CastMagicOnField(Character *caster, position pos, unsigned char ltastate) {
    fuse_ptr<Character> fuse_caster(caster);
    callEntrypoint("CastMagicOnField", fuse_caster, pos, ltastate);
}

void LuaMagicScript::CastMagicOnItem(Character *caster, ScriptItem TargetItem, unsigned char ltastate) {
    fuse_ptr<Character> fuse_caster(caster);
    callEntrypoint("CastMagicOnItem", fuse_caster, TargetItem, ltastate);
}

bool LuaMagicScript::actionDisturbed(Character *performer, Character *disturber) {
    fuse_ptr<Character> fuse_performer(performer);
    fuse_ptr<Character> fuse_disturber(disturber);
    return callEntrypoint<bool>("actionDisturbed", fuse_performer, fuse_disturber);
}

