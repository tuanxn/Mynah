// Student ID: 20274909

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Automaton.h"

using namespace std;

//int main()
//{
//   Automaton test = Automaton(1, 1);
//   cout << test.get_first_n_generations(10, 17);
//   cout << test.get_first_n_generations(10, 7);
//}

Automaton::Automaton(size_t num_parents, size_t rule) {
   if (num_parents > MAX_PARENTS) {
      _is_valid = false;
      return;
   }
   _num_parents = num_parents;
   _rules.resize(pow_2(_num_parents));

   if (!set_rule(rule)) {
      _is_valid = false;
      return;
   }

   _extreme_bit = 0;
}


bool Automaton::set_rule(size_t rule) {
   if (_num_parents > MAX_PARENTS || pow_2(pow_2(_num_parents))-1 < rule) {
      return false;
   }
   for (int i = 0; i < (signed)_rules.size(); i++) {
      _rules[i] = rule % 2;
      rule = rule / 2;
   }
   _is_valid = true;
   return true;
}

bool Automaton::equals(const Automaton& that) {
   if (!(this->_is_valid) && !(that._is_valid)) {
      return true;
   }
   else if (this->_is_valid && that._is_valid &&
      this->_num_parents == that._num_parents &&
      this->_extreme_bit == that._extreme_bit && this->_rules == that._rules) {
      return true;
   }
   return false;
}

bool Automaton::make_next_gen(const vector<int>& current_gen, vector<int>& next_gen) {
   next_gen.clear();
   
   if (!this->_is_valid || (current_gen.size() > 0 && current_gen.size() % 2 == 0)) {
      return false;
   }

   

   if (current_gen.size() == 0) {
      next_gen.push_back(1);
      return true;
   }
   for (int i = 0; i < (signed)(current_gen.size()+_num_parents-1); i++) {
      vector<int> temp;
      
      for (int j = i - _num_parents+1; j <= i; j++) {
         if (j < 0) {
            temp.push_back(_extreme_bit);
         }
         else if (j < (signed)current_gen.size()){
            temp.push_back(current_gen[j]);
         }
         else {
            temp.push_back(_extreme_bit);
         }
      }

      next_gen.push_back(_rules[translate_n_bits_starting_at(temp, 0, _num_parents)]);
      
   }

   if (_extreme_bit == 1) {
      _extreme_bit = _rules[_rules.size()-1];
   }
   else {
      _extreme_bit = _rules[0];
   }

   return true;
}

string Automaton::get_first_n_generations(size_t n, size_t width) {
   _extreme_bit = 0;
   
   if (width % 2 == 0) {
      return "";
   }
   
   string gen_output = "";

   vector<int> curr_gen;
   vector<int> next;

   for (size_t i = 0; i < n; i++) {
      if (make_next_gen(curr_gen, next)) {
         gen_output += (generation_to_string(next, width) + "\n");
         curr_gen = next;
      }
   }

   return gen_output;

}

string Automaton::generation_to_string(const vector<int>& gen, size_t width) {
   if (width % 2 == 0) {
      return "";
   }

   string gen_string = "";

   if (!_is_valid) {
      return gen_string;
   }

   size_t gen_size = gen.size();
   int pad_count = width - gen_size;
   string extreme_bit_str = " ";

   if (_extreme_bit == 1) {
      extreme_bit_str = "*";
   }

   if (pad_count > 0) {
      for (size_t i = 0; i < (pad_count / 2); i++) {
         gen_string += extreme_bit_str;
      }
   }

   if (pad_count < 0) {
      for (size_t j = -(pad_count / 2); j < (gen_size + (pad_count / 2)); j++) {
         if (gen[j] == 1) {
            gen_string += "*";
         }
         else {
            gen_string += " ";
         }
      }
   }
   else {
      for (size_t k = 0; k < gen_size; k++) {
         if (gen[k] == 1) {
            gen_string += "*";
         }
         else {
            gen_string += " ";
         }
      }
   }

   if (pad_count > 0) {
      for (size_t l = 0; l < (pad_count / 2); l++) {
         gen_string += extreme_bit_str;
      }
   }

   return gen_string;
}

size_t Automaton::translate_n_bits_starting_at(const vector<int>& bits, size_t pos, size_t n) {
   if (n < 1 || ((pos+n) > bits.size())) {
      return 0;
   }

   size_t decimal_total = 0;
   size_t offset = 0;

   for (size_t i = pos; i < (pos + n); i++) {
      decimal_total += pow_2(n-1-offset) * bits[i];
      offset++;
   }

   return decimal_total;
}