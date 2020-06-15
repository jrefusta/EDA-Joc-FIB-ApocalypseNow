#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */

#define PLAYER_NAME Jota
    
   const pair<int,int> T = make_pair (0,1);
   const pair<int,int> B = make_pair (0,-1);
   const pair<int,int> L = make_pair (-1,0);
   const pair<int,int> R = make_pair (1,0);
   const pair<int,int> TL = make_pair (-1,1);
   const pair<int,int> BL = make_pair (-1,-1);
   const pair<int,int> TR = make_pair (1,1);
   const pair<int,int> BR = make_pair (1,-1);
   
                                       // 0      1     2      3     4       5      6      7
   const pair<int,int> Directions[8] = {{0,1},{0,-1},{-1,0},{1,0},{-1,1},{-1,-1},{1,1},{1,-1}};
                                       // T      B     L      R     TL      BL     TR    BR
   typedef vector<int> VE;

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */  
  
      
  
  bool heli_valid_pos(int i, int j, int o) {
      int x, y;
      int n, m;
      switch(o) {
          case 3://b
              y = j-2;
              for(int h = -2; h < 3; ++h) {
                  x = i+h;
                  if (!pos_ok(x,y) or what(x,y) == MOUNTAIN) return false;
              }
              n = j-4;
              for (int l = -4; l < 5; ++l) {
                  m = i+l;
                  if (!pos_ok(m,n) or which_helicopter(m,n) != 0) return false;
              }
              
          case 0://r
              x = i+2;
              for(int h = -2; h < 3; ++h) {
                  y = j+h;
                  if (!pos_ok(x,y) or what(x,y) == MOUNTAIN) return false;
              }
              m = i+4;
              for (int l = -4; l < 5; ++l) {
                  n = j+l;
                  if (!pos_ok(m,n) or which_helicopter(m,n) != 0) return false;
              }
              
          case 1://t
              y = j+2;
              for(int h = -2; h < 3; ++h) {
                  x = i+h;
                  if (!pos_ok(x,y) or what(x,y) == MOUNTAIN) return false;
              }
              n = j+4;
              for (int l = -4; l < 5; ++l) {
                  m = i+l;
                  if (!pos_ok(m,n) or which_helicopter(m,n) != 0) return false;
              }
          case 2://l
              x = i-2;
              for(int h = -2; h < 3; ++h) {
                  y = j+h;
                  if (!pos_ok(x,y) or what(x,y) == MOUNTAIN) return false;
              }
              m = i-4;
              for (int l = -4; l < 5; ++l) {
                  n = j+l;
                  if (!pos_ok(m,n) or which_helicopter(m,n) != 0) return false;
              }
      }
      return true;
  }
      
  
  bool sol_valid_pos(int player, int i, int j) {
    int id2 = which_soldier(i,j);
    if (!pos_ok(i,j) or what(i,j) == WATER or what(i,j) == MOUNTAIN or fire_time(i,j) != 0) return false;
    if (id2 > 0 and data(id2).player == player) return false;
    return true;
    }
  
  
  void throw_parachuter(int player, int helicopter_id) {
    Position pos = data(helicopter_id).pos;
    for(int i = -REACH; i <= REACH; ++i) {
        for (int j = -REACH; j <= REACH; ++j) {
            int ii = pos.i + i;
            int jj = pos.j + j;
            if (sol_valid_pos(player,ii,jj)) {command_parachuter(ii,jj); return;}
        }
    }
  }
  
  
  
  bool conquerable_post(int player, int i, int j) {
      return ((post_owner(i,j) > -2) and (post_owner(i,j) != player));
  }
  
  void bfs_soldier(int player, int id) {
      bool attack = false;
      Position actual = data(id).pos;
        for (int x = 0; x < 8 and !attack; ++x) {
            int ii = actual.i + Directions[x].first;
            int jj = actual.j + Directions[x].second;
            if (pos_ok(ii,jj)) {
                int id2 = which_soldier(ii,jj);
	            if (id2 > 0 and data(id2).player != player) {
                    command_soldier(id,ii,jj);
                    attack = true;
                }
            }
        }
        if (!attack){
      Position next = actual;
      queue<Position> Q;
      Q.push(actual);
      vector<VE> Map(MAX, VE(MAX,-1));
      Map[actual.i][actual.j] = 0;
      bool post_found = false;
      while (!Q.empty()) {
          Position c = Q.front();
          Q.pop();
          if (post_found) {
              Position aux;
              while (Map[c.i][c.j] != 0) {
                  bool out = false;
                  for (int j = 0; j < 8 and !out; ++j) {
                      aux.i = c.i + Directions[j].first;
                      aux.j = c.j + Directions[j].second;
                      if (Map[aux.i][aux.j] == (Map[c.i][c.j])-1) c = aux;
                      if (Map[c.i][c.j] == 0) {
                          switch (j) {
                              case 0:
                                  next.i += B.first;
                                  next.j += B.second;
                                  break;
                              case 1:
                                    next.i += T.first;
                                    next.j += T.second;
                                    break;
                              case 2:
                                  next.i += R.first;
                                  next.j += R.second;
                                  break;
                              case 3:
                                  next.i += L.first;
                                  next.j += L.second;
                                  break;
                              case 4:
                                  next.i += BR.first;
                                  next.j += BR.second;
                                  break;
                              case 5:
                                  next.i += TR.first;
                                  next.j += TR.second;
                                  break;
                              case 6:
                                  next.i += BL.first;
                                  next.j += BL.second;
                                  break;
                              case 7:
                                  next.i += TL.first;
                                  next.j += TL.second;
                                  break;
                          }
                          out = true;
                          
                      }
                  }
              }
          }
          else {             
              for (int i = 0; i < 8 and !post_found; ++i) {
                  int ii = c.i + Directions[i].first;
                  int jj = c.j + Directions[i].second;
                  if(sol_valid_pos(player,ii,jj) and Map[ii][jj] == -1) {
                      Position aux;
                      aux.i = ii;
                      aux.j = jj;
                      if (conquerable_post(player,ii,jj)) {
                          post_found  = true;
                          while (!Q.empty()) Q.pop();
                          Q.push(aux);
                          Map[aux.i][aux.j] = Map[c.i][c.j]+1;
                      }
                      else {
                          Q.push(aux);
                          Map[aux.i][aux.j] = Map[c.i][c.j]+1;
                      }
                  }
              }
          }
      }
      command_soldier(id,next.i,next.j);
  }
      
  }
      

  void play_helicopter(int player, int id) {
      if (not data(id).parachuters.empty()) {
          throw_parachuter(player,id);
      }
      Position pos = data(id).pos;
      int enemies = 0;
      int allies = 0;
      for (int i = -REACH; i <= REACH; ++i) {
          for (int j = -REACH; j <= REACH; ++j) {
              int ii = pos.i + i;
              int jj = pos.j + j;
              int someone = which_soldier(ii,jj);
              if (pos_ok(ii,jj) and someone > 0) {
                  if (data(someone).player == player) ++allies;
                  if (data(someone).player != player) ++enemies;
              }
          }
      }
      if ((enemies >= allies+2) and data(id).napalm == 0) {
          command_helicopter(id,NAPALM);
          return;
      }
      
      int ii = pos.i;
      int jj = pos.j;
      int o = data(id).orientation;
      bool forward = false;
      switch (o) {
          case 0://r
              ii += R.first;
              jj += R.second;
              if (heli_valid_pos(ii,jj,o)) forward = true;
              else forward = false;
              break;
          case 1://t
              ii += T.first;
              jj += T.second;
              if (heli_valid_pos(ii,jj,o)) forward = true;
              else forward = false;
              break;
          case 2://l
              ii += L.first;
              jj += L.second;
              if (heli_valid_pos(ii,jj,o)) forward = true;
              else forward = false;
              break;
          case 3://b
              ii += B.first;
              jj += B.second; 
              if (heli_valid_pos(ii,jj,o)) forward = true;
              else forward = false;
              break;
      }
      if (forward == true) command_helicopter(id,FORWARD1);
      else command_helicopter(id,CLOCKWISE);
  }
              
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {

    int player = me();
    VE H = helicopters(player); // helicopters of my player
    VE S = soldiers(player); //    soldiers of my player

    bool atack = false;
    for (int i = 0; i < (int)H.size(); ++i) play_helicopter(player, H[i]);
    for (int i = 0; i < (int)S.size(); ++i) {/*
        Position s = data(S[i]).pos;
        for (int x = 0; x < 8 and !atack; ++x) {
            int ii = s.i + Directions[x].first;
            int jj = s.j + Directions[x].second;
            if (pos_ok(ii,jj)) {
                int id2 = which_soldier(ii,jj);
	            if (id2 > 0 and data(id2).player != player) {
                    command_soldier(S[i],ii,jj);
                    atack = true;
                }
            }
        }
        if (!atack) */bfs_soldier(player,S[i]);
    }
    }

};
/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
