import sys
import math

# Grab Snaffles and try to throw them through the opponent's goal!
# Move towards a Snaffle to grab it and use your team id to determine towards where you need to throw it.
# Use the Wingardium spell to move things around at your leisure, the more magic you put it, the further they'll move.

my_team_id = int(input())  # if 0 you need to score on the right of the map, if 1 you need to score on the left

class Entity:
    dist_wiz0 = None
    dist_wiz1 = None
    bludge_way = 0
    def __init__(self, id, type, x, y, vx, vy, state, dist_x, my_vx):
        self.id = id
        self.type = type
        self.x = x
        self.y = y
        self.vx = vx
        self.vy = vy
        self.state = state
        self.dist_x = dist_x
        self.my_vx = my_vx

def sort_snaffles_w0(obj):
    return(obj.dist_wiz0)

def sort_snaffles_w1(obj):
    return(obj.dist_wiz1)
    
def sort_snaffles_x(obj):
    return(obj.dist_x)

  
throw0 = 0
throw1 = 0
if my_team_id == 0:
    goal_x = 16000
else:
    goal_x = 0
loop = 1

# game loop
while True:
    my_score, my_magic = [int(i) for i in input().split()]
    opponent_score, opponent_magic = [int(i) for i in input().split()]
    entities = int(input())  # number of entities still in game
    lstobj = []
    for i in range(entities):
        # entity_id: entity identifier
        # entity_type: "WIZARD", "OPPONENT_WIZARD" or "SNAFFLE" or "BLUDGER"
        # x: position
        # y: position
        # vx: velocity
        # vy: velocity
        # state: 1 if the wizard is holding a Snaffle, 0 otherwise. 1 if the Snaffle is being held, 0 otherwise. id of the last victim of the bludger.
        # print(i, file=sys.stderr)
        entity_id, entity_type, x, y, vx, vy, state = input().split()
        # print(entity_id, file=sys.stderr)
        # print(entity_type, file=sys.stderr)
        entity_id = int(entity_id)
        x = int(x)
        y = int(y)
        vx = int(vx)
        vy = int(vy)
        state = int(state)
        if my_team_id == 0:
            dist_x = x
            my_vx = vx
        else:
            dist_x = 16000 - x
            my_vx = -vx
        lstobj.append(Entity(entity_id, entity_type, x, y, vx, vy, state, dist_x, my_vx))
   

    ################################################
    #get wizz_bludge, the id of the wizz who is targeted:
    #1) get bludge, the id of the bludge 2) get wiz_bludge
    lst_bludge = [i for i in lstobj if i.type == "BLUDGER"]
    if my_team_id == 0:
        bludge = lst_bludge[0]
    else:
        bludge = lst_bludge[1]
    #print(bludge.id, file=sys.stderr)
    
    if bludge.vy > 0:
        if my_team_id == 0:
            wiz_bludge = 0
        else:
            wiz_bludge = 3
    else:
        if my_team_id == 0:
            wiz_bludge = 1
        else:
            wiz_bludge = 2
    #print(wiz_bludge, file=sys.stderr)

   
   ################################################
   #sort snaffles by distance vs w0 and vs w1
    lst_snaf = [i for i in lstobj if i.type == "SNAFFLE"]
    
    if loop > 2 and loop < 10:
        for obj in lst_snaf:
            if wiz_bludge == 0 or wiz_bludge == 2:
                cord_x = obj.x * -0.21 + 5460
            else:
                cord_x = obj.x * 0.21 + 2070
            #print(str(obj.x / 4 + 2000), file=sys.stderr)
            if cord_x > (obj.y - 800) and cord_x < (obj.y + 800):
                #print(obj.id, file=sys.stderr)
                obj.bludge_way = 1
        lst_snaf= [i for i in lst_snaf if i.bludge_way == 0]
        #for the first loops (3 to 9), suppress wiz that are on the blugers way from the list    
    
    for i in lst_snaf:
        print(i.id, file=sys.stderr)
        i.dist_wiz0 = (i.x - lstobj[0].x)**2 + (i.y - lstobj[0].y)**2
        i.dist_wiz1 = (i.x - lstobj[1].x)**2 + (i.y - lstobj[1].y)**2
    
    
    
    lst_snaf_w0 = sorted(lst_snaf, key = sort_snaffles_w0)
    lst_snaf_w1 = sorted(lst_snaf, key = sort_snaffles_w1)
    #w0 and w1 must not follow the same snaf. If the closest snaf is the same for both, w1 will follow the second closest to him
    diff0 = 0
    diff1 = 0
    if entities <= 7:
        diff0 = 0
        diff1 = 0
    elif lst_snaf_w0[0].id == lst_snaf_w1[0].id:
        if lst_snaf_w0[0].dist_wiz0 >= lst_snaf_w1[0].dist_wiz1:
            diff0 = 1
        else:
            diff1 = 1
    ################################################
    #sort snaffles by distance to my own goal
    lst_snaf_x = sorted(lst_snaf, key = sort_snaffles_x)
    
    #campos variable set to one if a snaffle is close to the opponent goal and behind both opponents wizz
    if lst_snaf_x[-1].dist_x > 12000 and lst_snaf_x[-1].dist_x > lstobj[2].dist_x and lst_snaf_x[-1].dist_x > lstobj[3].dist_x:
        campos = 1
    else:
        campos = 0
        
    #defend set to one if a snaffle is close to my goal and moving towards it and behind both of my wizz
    if lst_snaf_x[0].dist_x < 4000 and lst_snaf_x[0].my_vx < -100 and lst_snaf_x[0].dist_x < lstobj[0].dist_x and lst_snaf_x[0].dist_x < lstobj[1].dist_x:
        defend = 1
    else:
        defend = 0
    
    ################################################
    #if ball_y is inside goal, throw on y = ball_y
    #if ball is inside corner, throw on opposite side of the goal
    #WINGARDIUM 
    attack_y = 3750
    if lst_snaf_x[-1].dist_x > 14500 and lst_snaf_x[-1].y < 1200:
        attack_y = 4700
    if lst_snaf_x[-1].dist_x > 14500 and lst_snaf_x[-1].y > 6300:
        attack_y = 2800
    if lst_snaf_x[-1].dist_x > 15000 and lst_snaf_x[-1].y > 1800 and lst_snaf_x[-1].y < 5700:
        attack_y = lst_snaf_x[-1].y
    
    #SAME FOR THROW
    goal_y0 = 3750
    if lstobj[0].dist_x > 14500 and lstobj[0].y < 1200:
        goal_y0 = 4700
    if lstobj[0].dist_x > 14500 and lstobj[0].y > 6300:
        goal_y0 = 2800
    if lstobj[0].dist_x > 15000 and lstobj[0].y > 1800 and lstobj[0].y < 5700:
        goal_y0 = lstobj[0].y
    
    goal_y1 = 3750
    if lstobj[1].dist_x > 14500 and lstobj[1].y < 1200:
        goal_y1 = 4700
    if lstobj[1].dist_x > 14500 and lstobj[1].y > 6300:
        goal_y1 = 2800
    if lstobj[1].dist_x > 15000 and lstobj[1].y > 1800 and lstobj[1].y < 5700:
        goal_y1 = lstobj[1].y
    
    ################################################
    #PASS
    
    same_y = 0
    if abs(lstobj[0].vy - lstobj[1].vy) < 150:
        same_y = 1
    dista = abs(lstobj[0].vy - lstobj[1].vy)
    #print(str(dista), file=sys.stderr)
    
    #throw to wiz1 if:
     #inside own 12k,  
     #wiz1 is "in front of" wiz0
     #distance is not too importnt
     #wiz1 is moving towards opp goal
     #both wizz are not going in different y directions (same_y == 1)
    
    i = lstobj[0]
    dist_wiz = (i.x - lstobj[1].x)**2 + (i.y - lstobj[1].y)**2
    dist_wiz = dist_wiz / 1000
    #print(str(dist_wiz), file=sys.stderr)
    
     
    pass_to_w1 = 0
    if same_y == 1:
        if lstobj[0].dist_x < 12000 and lstobj[1].dist_x > lstobj[0].dist_x + 10 and dist_wiz < 13000 and lstobj[1].my_vx > 0 and lstobj[0].my_vx >0:
            pass_to_w1 = 1
    
    pass_to_w0 = 0
    if same_y == 1:
        if lstobj[1].dist_x < 12000 and lstobj[0].dist_x > lstobj[1].dist_x + 10 and  dist_wiz < 13000 and lstobj[0].my_vx > 0 and lstobj[1].my_vx > 0:
            pass_to_w0 = 1
    
    #throw in direction of wiz + coef * v (coef increases with distance)
    if dist_wiz < 2000:
        pass_coef = 0
    if dist_wiz < 5000:
        pass_coef = 2
    elif dist_wiz < 10000:
        pass_coef = 4
    else:
        pass_coef = 6
    
    #print(str(pass_coef), file=sys.stderr)
    

    
    ################################################    
    spell = 1
    #print(lstobj[4].my_vx, file=sys.stderr)
    
    
    
    for i in range(2):

        if i == 2:
            j = 0
            for to in lstobj:
                print(str(j), file=sys.stderr)
                print(str(to.id), file=sys.stderr)
                print(to.type, file=sys.stderr)
                j = j + 1
        #print("x coord de 4" + str(lstobj[4].x), file=sys.stderr)
        if i == 0:
            if lstobj[0].state == 1:
                throw0 = 1
            if throw0 == 1:
                if pass_to_w1 == 1:
                    print("THROW " + str(lstobj[1].x + lstobj[0].vx * pass_coef) + " " + str(lstobj[1].y + lstobj[1].vy * pass_coef) + " 500")
                    print("pass to w1", file=sys.stderr)
                else:
                    print("THROW " + str(goal_x) + " " + str(goal_y0) + " 500")
                throw0 = 0
            elif my_magic > 20 and defend == 1:
                print("WINGARDIUM " + str(lst_snaf_x[0].id) + " " + str(goal_x) + " 3750 " + str(my_magic))
                defend = 0
            elif my_magic > 20 and campos == 1:
                print("WINGARDIUM " + str(lst_snaf_x[-1].id) + " " + str(goal_x) + " " + str(attack_y) + " " + str(my_magic))
                campos = 0
            elif my_magic > 50:
                print("WINGARDIUM " + str(lst_snaf_w0[0 + diff0].id) + " " + str(goal_x) + " 3750 16")
                spell = 0
            else:
                print("MOVE " + str(lst_snaf_w0[0 + diff0].x) + " " + str(lst_snaf_w0[0 + diff0].y) + " 150")
        if i == 1:
            if lstobj[1].state == 1:
                throw1 = 1
            if throw1 == 1:
                if pass_to_w0 == 1:
                    print("THROW " + str(lstobj[0].x + lstobj[0].vx * pass_coef) + " " + str(lstobj[0].y + lstobj[0].vy * pass_coef) + " 500")
                    print("pass to w0", file=sys.stderr)
                else:
                    print("THROW " + str(goal_x) + " " + str(goal_y1) + " 500")
                throw1 = 0
            elif my_magic > 20 and defend == 1:
                print("WINGARDIUM " + str(lst_snaf_x[0].id) + " " + str(goal_x) + " " + str(attack_y) + " " + str(my_magic))
            elif my_magic > 20 and campos == 1:
                print("WINGARDIUM " + str(lst_snaf_x[-1].id) + " " + str(goal_x) + " 3750 " + str(my_magic))
            elif my_magic > 50 and spell == 1:
                print("WINGARDIUM " + str(lst_snaf_w1[0 + diff1].id) + " " + str(goal_x) + " 3750 16")
            else:
                print("MOVE " + str(lst_snaf_w1[0 + diff1].x) + " " + str(lst_snaf_w1[0 + diff1].y) + " 150")
    
    loop = loop + 1
