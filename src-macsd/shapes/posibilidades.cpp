template <typename T>
posibilidades<T>::posibilidades(const vector< vector<T> >& cuales) : _options(cuales.size()) {
   _options = cuales;
//    sort(_options.begin(), _options.end());
//    // DEBUG
//    for (typename map<T,unsigned int>::iterator p = _dicc.begin(); p != _dicc.end(); p++)
//        cout << (*p).first << ' ' << (*p).second << endl;
//    for (unsigned int i = 0; i < cuales.size(); i++) {
//        for (unsigned int j = 0; j < cuales[i].size(); j++)
//            cout << _options[i][j] << ',';
//        cout << endl;
//    }
}
        
template <typename T>
typename posibilidades<T>::iterator posibilidades<T>::iterator::operator++() {  
    int ll = _cuales->size() - 1;
    _pos++;

    bool done = false; 
    bool found = false;       
    do {       
//         if (found) {
            _donde[ll]++;
            
            while (!done && (ll >= 0) && (_donde[ll] == (*_cuales)[ll].size())) {
    //             cout << "ANTES: " << ll << ' ' << donde[ll] << endl;
                _donde[ll] = 0;
                if (ll - 1 >= 0)
                    _donde[ll-1]++;
                ll--;
    //             cout << "DESPI: " << ll << ' ' << donde[ll] << endl;
            }
            if (ll < 0) {
                done = true;
                _pos = -1;
            }
//         }
             vector<T> v(_cuales->size());
        found = false;
        unsigned int i, j;
        for (i = 0; (i < _cuales->size()) && !found; i++) {
            v[i] = (*_cuales)[i][_donde[i]];
            for (j = 0; (j < i) && !found; j++) {
                found = (v[i] == v[j]);
            }
//             cout << _donde[i] << ',';
        }
//         cout << endl;
        
//         if (found) {
//             ll = i - 1;
//             for (unsigned int k = i; k < _cuales->size(); k++)
//                 _donde[k] = 0;
//         }
//         else
            ll = _cuales->size() - 1;
     }
    while (found && !done);
    
    return *this;
}

template <typename T>
vector<T> posibilidades<T>::iterator::operator*() {
   vector<T> res(_donde.size());
    
   cout << "OPCION: ";
   for (unsigned int i = 0; i < _donde.size(); i++) {
        res[i] = (*_cuales)[i][_donde[i]];
        cout << res[i] << ' ';
   }
   cout << endl;
    
   return res;
}

template <typename T>
posibilidades<T>::iterator::iterator(vector< vector<T> >* cuales, const int p) : _donde(cuales->size(), 0) {
    _pos = p;
    _cuales = cuales;
}
                
template <typename T>
bool posibilidades<T>::iterator::iterator::valid() const {
    bool res = true;
    
    for (unsigned int i = 0; (i < _donde.size()) && res; i++)
        for (unsigned int j = i + 1; (j < _donde.size()) && res; j++)
            res = ((*_cuales)[i][_donde[i]] != (*_cuales)[j][_donde[j]]);
    
    return res;
}

template <typename T>
typename posibilidades<T>::iterator posibilidades<T>::begin() {
    bool done = false;
    for (unsigned int i = 0; (i < _options.size()) && !done; i++) {
        done = (_options[i].size() == 0);
    }
    if ((_options.size() != 0) && !done) {
        typename posibilidades<T>::iterator x(&_options, 0);
        if (x.valid())
            return typename posibilidades<T>::iterator(&_options, 0);
        else
            return ++(typename posibilidades<T>::iterator(&_options, 0));
    }
    else
        return typename posibilidades<T>::iterator(-1);
}

template <typename T>
typename posibilidades<T>::iterator posibilidades<T>::end() {
    return typename posibilidades<T>::iterator(-1);
}
