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


#ifndef TCCHARACTERVECTOR_HH
#define TCCHARACTERVECTOR_HH

#include <string>
#include <vector>
#include "utility.hpp"

//! eine std::vector-Template-Klasse f�r Character-Objekte
// Die Klasse erweitert die Funktionalit�t von std::vector um
// ccharacter - spezifische Suchfunktionen
template < class _Tp >
class ccharactervector : public std::vector < _Tp > {
public:
    //! sucht in dem std::vector nach dem Character mit Namen n
    // \param n der Name des Character
    // \return _Tp Zeiger auf den gefunden Character, NULL falls nicht gefunden
    _Tp find(std::string n);

    //! sucht in dem std::vector nach dem Character mit der ID id
    // \param id die id des Character
    // \return _Tp Zeiger auf den gefunden Character, NULL falls nicht gefunden
    _Tp findID(TYPE_OF_CHARACTER_ID id);

    //! sucht in dem std::vector nach dem Character mit den Koordinaten xc,yc,zc
    // \param xc X-Koordinate des gesuchten Character
    // \param xc Y-Koordinate des gesuchten Character
    // \param xc Z-Koordinate des gesuchten Character
    // \return _Tp Zeiger auf den gefunden Character, NULL falls nicht gefunden
    _Tp find(short int xc, short int yc, short int zc);

    //! sucht in dem std::vector nach dem Character mit den Koordinaten xc,yc,zc
    // \param xc X-Koordinate des gesuchten Character
    // \param xc Y-Koordinate des gesuchten Character
    // \param xc Z-Koordinate des gesuchten Character
    // \param ret Zeiger auf den gefunden Character
    // \return true falls ein Character gefunden wurde, false sonst
    bool find(short int xc, short int yc, short int zc, _Tp &ret);

    //! l�scht in dem std::vector den Character mit den Koordinaten xc,yc,zc
    // \param xc X-Koordinate des gesuchten Character
    // \param xc Y-Koordinate des gesuchten Character
    // \param xc Z-Koordinate des gesuchten Character
    // \return true falls ein Character gel�scht wurde, false sonst
    bool remove(short int xc, short int yc, short int zc);

    //! sucht in dem std::vector nach dem Character mit der Id id
    // \param id die ID des gesuchten Character
    // \param newIt falls erfolgreich der Iterator auf den Character mit der ID id
    // \return true falls der Character gefunden wurde, false sonst
    bool getIterator(TYPE_OF_CHARACTER_ID id, typename ccharactervector::iterator &newIt);

    //! sucht in dem std::vector nach Character mit Koordinaten in der N�he von (xc,yc,zc)
    // \param rnorth maximaler Abstand der Character nach Norden
    // \param rsouth maximaler Abstand der Character nach S�den
    // \param reast maximaler Abstand der Character nach Osten
    // \param rwest maximaler Abstand der Character nach Westen
    // \param rup maximaler Abstand der Character nach oben
    // \param rdown maximaler Abstand der Character nach unten
    // \return std::vector<_Tp> Vektor mit allen Character mit Koordinate (x,y,z) f�r die gilt:
    // (zc-rup <= z <= zc+down) und (xc-rwest <= x <= xc+reast) und (yc-rnorth <= y <= yc+rwest)
    std::vector < _Tp > findAllCharactersInRangeOf(short int xc, short int yc, short int zc, int distancemetric);
    std::vector < _Tp > findAllCharactersInMaxRangeOf(short int xc, short int yc, short int zc, int distancemetric);

    std::vector < _Tp > findAllCharactersInScreen(short int xc, short int yc, short int zc);

    //! sucht in dem std::vector nach lebenden Character mit Koordinaten in der N�he von (xc,yc,zc)
    // \param rnorth maximaler Abstand der Character nach Norden
    // \param rsouth maximaler Abstand der Character nach S�den
    // \param reast maximaler Abstand der Character nach Osten
    // \param rwest maximaler Abstand der Character nach Westen
    // \param rup maximaler Abstand der Character nach oben
    // \param rdown maximaler Abstand der Character nach unten
    // \return std::vector<_Tp> Vektor mit allen Character mit Koordinate (x,y,z) f�r die gilt:
    // (zc-rup <= z <= zc+down) und (xc-rwest <= x <= xc+reast) und (yc-rnorth <= y <= yc+rwest)
    std::vector < _Tp > findAllAliveCharactersInRangeOf(short int xc, short int yc, short int zc, int distancemetric);

    /**
    * looks in the vector for all characters which are alive and coordinates in the near of the given coordinates
    * but only at the same map ( z = z )
    */
    std::vector < _Tp > findAllAliveCharactersInRangeOfOnSameMap(short int xc, short int yc, short int zc, int distancemetric);

    //! sucht in dem std::vector nach Character mit X-Koordinaten f�r die gilt startx <= x <= endx
    // \param startx kleinste X-Koordinate f�r akzeptierte Character
    // \param endx gr��te X-Koordinate f�r akzeptierte Character
    // \return true falls mindestens ein Character gefunden wurde
    bool findAllCharactersWithXInRangeOf(short int startx, short int endx, std::vector < _Tp > &ret);

};


template < class _Tp > _Tp ccharactervector < _Tp > ::find(std::string n) {

    typename ccharactervector::iterator thisIterator;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        if (comparestrings_nocase((*thisIterator)->name, n)) {
            return * thisIterator;
        }
    }

    return NULL;

}


template < class _Tp > _Tp ccharactervector < _Tp > ::findID(TYPE_OF_CHARACTER_ID id) {
    typename ccharactervector::iterator thisIterator;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        if (((*thisIterator)->id) == id) {
            return * thisIterator;
        }
    }

    return NULL;

}


template < class _Tp > _Tp ccharactervector < _Tp > ::find(short int xc, short int yc, short int zc) {
    typename ccharactervector::iterator thisIterator;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        if (((*thisIterator)->pos.x) == xc) {
            if (((*thisIterator)->pos.y) == yc) {
                if (((*thisIterator)->pos.z) == zc) {
                    return * thisIterator;
                }// z
            }// y
        }// x
    }// iterator

    return NULL;

}


template < class _Tp > bool ccharactervector < _Tp > ::find(short int xc, short int yc, short int zc, _Tp &ret) {

    typename ccharactervector::iterator thisIterator;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        if (((*thisIterator)->pos.x) == xc) {
            if (((*thisIterator)->pos.y) == yc) {
                if (((*thisIterator)->pos.z) == zc) {
                    ret = *thisIterator;
                    return true;
                }// z
            }// y
        }// x
    }// iterator

    return false;

}


template < class _Tp > bool ccharactervector < _Tp > ::remove(short int xc, short int yc, short int zc) {

    typename ccharactervector::iterator thisIterator;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        if (((*thisIterator)->pos.x) == xc) {
            if (((*thisIterator)->pos.y) == yc) {
                if (((*thisIterator)->pos.z) == zc) {
                    erase(thisIterator);
                    return true;
                }// z
            }// y
        }// x
    }// iterator

    return false;

}


template < class _Tp > bool ccharactervector < _Tp > ::getIterator(TYPE_OF_CHARACTER_ID id, typename ccharactervector::iterator &newIt) {

    typename ccharactervector::iterator thisIterator;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        if (((*thisIterator)->id) == id) {
            newIt = thisIterator;
            return true;
        }
    }

    return false;

}


template < class _Tp > std::vector < _Tp > ccharactervector < _Tp > ::findAllCharactersInRangeOf(short int xc, short int yc, short int zc, int distancemetric) {

    std::vector < _Tp > temp;
    typename ccharactervector::iterator thisIterator;
    short int px;
    short int py;
    short int pz;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        pz = (*thisIterator)->pos.z - zc;

        if ((-RANGEDOWN <= pz) && (pz <= RANGEUP)) {
            px = (*thisIterator)->pos.x - xc;
            py = (*thisIterator)->pos.y - yc;

            if ((abs(px) + abs(py)) <= distancemetric) {
                temp.push_back(*thisIterator);
            }
        }// z
    }// iterator

    return temp;

}


template < class _Tp > std::vector < _Tp > ccharactervector < _Tp > ::findAllCharactersInScreen(short int xc, short int yc, short int zc) {

    std::vector < _Tp > temp;
    typename ccharactervector::iterator thisIterator;
    short int px;
    short int py;
    short int pz;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        pz = (*thisIterator)->pos.z - zc;

        if ((-RANGEDOWN <= pz) && (pz <= RANGEUP)) {
            px = (*thisIterator)->pos.x - xc;
            py = (*thisIterator)->pos.y - yc;

            if ((abs(px) + abs(py)) <= (*thisIterator)->getScreenRange()) {
                temp.push_back(*thisIterator);
            }
        }// z
    }// iterator

    return temp;

}


template < class _Tp > std::vector < _Tp > ccharactervector < _Tp > ::findAllCharactersInMaxRangeOf(short int xc, short int yc, short int zc, int distancemetric) {

    std::vector < _Tp > temp;
    typename ccharactervector::iterator thisIterator;
    short int px;
    short int py;
    short int pz;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        pz = (*thisIterator)->pos.z - zc;

        if ((-RANGEDOWN <= pz) && (pz <= RANGEUP)) {
            px = (*thisIterator)->pos.x - xc;
            py = (*thisIterator)->pos.y - yc;

            if ((abs(px) <= distancemetric) && (abs(py) <=distancemetric)) {
                temp.push_back(*thisIterator);
            }
        }// z
    }// iterator

    return temp;

}


template < class _Tp > std::vector < _Tp > ccharactervector < _Tp > ::findAllAliveCharactersInRangeOf(short int xc, short int yc, short int zc, int distancemetric) {
    std::vector < _Tp > temp;
    typename ccharactervector::iterator thisIterator;
    short int px;
    short int py;
    short int pz;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        pz = (*thisIterator)->pos.z - zc;

        if ((-RANGEDOWN <= pz) && (pz <= RANGEUP)) {
            px = (*thisIterator)->pos.x - xc;
            py = (*thisIterator)->pos.y - yc;

            if (((abs(px) + abs(py)) <= distancemetric) ||
                ((distancemetric == 1) && (abs(px) == 1) && (abs(py) == 1))) {       // Allow angle attacks
                if ((*thisIterator)->IsAlive()) {
                    temp.push_back(*thisIterator);
                }
            }
        }// z
    }// iterator

    return temp;
}

template < class _Tp > std::vector < _Tp > ccharactervector < _Tp > ::findAllAliveCharactersInRangeOfOnSameMap(short int xc, short int yc, short int zc, int distancemetric) {
    std::vector < _Tp > temp;
    typename ccharactervector::iterator thisIterator;
    short int px;
    short int py;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        //z coordinate is the same
        if ((*thisIterator)->pos.z == zc) {
            px = (*thisIterator)->pos.x - xc;
            py = (*thisIterator)->pos.y - yc;

            if (((abs(px) + abs(py)) <= distancemetric) || ((distancemetric == 1) && (abs(px) == 1) && (abs(py) == 1))) {          // Allow angle attacks
                if ((*thisIterator)->IsAlive()) {
                    temp.push_back(*thisIterator);
                }
            }
        }
    }// iterator

    return temp;
}


template < class _Tp > bool ccharactervector < _Tp > ::findAllCharactersWithXInRangeOf(short int startx, short int endx, std::vector < _Tp > &ret) {
    typename ccharactervector::iterator thisIterator;
    bool found_one = false;

    for (thisIterator = this->begin(); thisIterator < this->end(); ++thisIterator) {
        if (((*thisIterator)->pos.x >= startx) && ((*thisIterator)->pos.x <= endx)) {
            ret.push_back(*thisIterator);
            found_one = true;
        }// x
    }// iterator

    return found_one;

}

#endif
