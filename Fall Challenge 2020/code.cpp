#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/


class spells
{
    public:
    int actionId; // the unique ID of this spell or recipe
    int delta0; // tier-0 ingredient change
    int delta1; // tier-1 ingredient change
    int delta2; // tier-2 ingredient change
    int delta3; // tier-3 ingredient change
    bool castable; // 1 if this is a castable player spell
    bool repeatable;
    int tome_index;
    int tax_count;
    int sum(void)
    {
        return(this->delta0 + this->delta1 + this->delta2 + this->delta3);
    }
    int score()
    {
        return(this->delta0 * 0.5 + this->delta1 * 2 + this->delta2 * 3 + this->delta3 * 4);
    }
    bool ascend_or_flat()
    {
        if (this->null_deltas() == 2 && this->delta3 >= 0)
        {
            return true;
        }
        else if (this->null_deltas() == 3)
        {
            return true;
        }
        else if (this->neg_deltas() == 0)
        {
            return true;

        }
        else
            return false;
    }
    int null_deltas()
    {
        int ret(0);
        if (this->delta0 == 0)
            ret++;
        if (this->delta1 == 0)
            ret++;
        if (this->delta2 == 0)
            ret++;
        if (this->delta3 == 0)
            ret++;
        return ret;
    }
    int neg_deltas()
    {
        int ret(0);
        if (this->delta0 < 0)
            ret++;
        if (this->delta1 < 0)
            ret++;
        if (this->delta2 < 0)
            ret++;
        if (this->delta3 < 0)
            ret++;
        return ret;
    }
};

class recettes
{
    public:
    int actionId; // the unique ID of this spell or recipe
    int delta0; // tier-0 ingredient change
    int delta1; // tier-1 ingredient change
    int delta2; // tier-2 ingredient change
    int delta3; // tier-3 ingredient change
    int price; // the price in rupees if this is a potion
};

class inv
{
    public:
    int inv0; // tier-0 ingredients in inventory
    int inv1;
    int inv2;
    int inv3;
    int recipe_target_index;
    int recipe_target_actionid = -1;
    int target_0;
    int target_1;
    int target_2;
    int target_3;
    int num_spell = 0;
    int target_tier_for_round = -1;
    bool comb2_is_on = false;
    bool comb3_is_on = false;
    int next_comb_action;
    int next_next_comb_action;
    int sum_inv(void)
    {
        return (this->inv0 + this->inv1 + this->inv2 + this->inv3);
    }
    int sum_target(void)
    {
        return (this->target_0 + this->target_1 + this->target_2 + this->target_3);
    }
    int start_index()
    {
        if (this->inv0 == 0) //no more inv0
            return 0;
        if (this->inv3 >= this->target_3 && this->inv2 >= this->target_2 && this->inv1 >= this->target_1
        && this->inv0 < this->target_0) // enough of all invs, except inv0
            return 0;
        return 1;
    }
    int stop_index()
    {
        if (this->inv3 < this->target_3)
            return 4;
        if (this->inv2 < this->target_2)
            return 3;
        if (this->inv1 < this->target_1)
            return 2;
        return 1;
    }
    bool can_do_target_recipe()
    {
        if (this->inv0 - this->target_0 >= 0 && this->inv1 - this->target_1 >= 0 && this->inv2 - this->target_2 >= 0 && this->inv3 - this->target_3 >= 0)
            return true;
        return false;
    }
    bool can_do_spell(spells spell)
    {

        if (spell.delta0 < 0 && spell.delta0 + this->inv0 < 0)
            return false;
        if (spell.delta1 < 0 && spell.delta1 + this->inv1 < 0)
            return false;
        if (spell.delta2 < 0 && spell.delta2 + this->inv2 < 0)
            return false;
        if (spell.delta3 < 0 && spell.delta3 + this->inv3 < 0)
            return false;
        return true;
    }
    bool enough_room_for_spell(spells spell)
    {
        if (spell.sum() + this->sum_inv() > 10)
            return false;
        return true;
    }
    bool way_enough_room_for_spell(spells spell)
    {
        if (spell.sum() + this->sum_inv() > 8)
            return false;
        return true;
    }
    bool not_too_much_of_one(spells spell)
    {
        if (spell.delta0 + this->inv0 > 5 ||
        spell.delta1 + this->inv1 > 4 ||
        spell.delta2 + this->inv2 > 4 ||
        spell.delta3 + this->inv3 > 4)
            return false;
        return true;
    }
    bool tiers_still_needed()
    {
        if (this->sum_target() - this->sum_inv() > 0)
            return true;
        return false;
    }
    int effort(recettes recette)
    {
        int ret(0);
        if (this->inv0 + recette.delta0 < 0)
            ret -= 0.5 * (this->inv0 + recette.delta0);
        if (this->inv1 + recette.delta1 < 0)
            ret -= 2 * (recette.delta1 + this->inv1);
        if (this->inv2 + recette.delta2 < 0)
            ret -= 3 * (recette.delta2 + this->inv2);
        if (this->inv3 + recette.delta3 < 0)
            ret -= 4 * (recette.delta3 + this->inv3);
        return (ret);
    }
    int effort()
    {
        int ret(0);
        if (this->inv0 - this->target_0 < 0)
            ret -= 0.5 * (this->inv0 - this->target_0);
        if (this->inv1 - this->target_1 < 0)
            ret -= 2 * (this->inv1 - this->target_1);
        if (this->inv2 - this->target_2 < 0)
            ret -= 3 * (this->inv2 - this->target_2);
        if (this->inv3 - this->target_3 < 0)
            ret -= 4 * (this->inv3 - this->target_3);
        return (ret);
    }
    int projected_effort(spells spell)
    {
        int ret(0);
        if (this->inv0 + spell.delta0 - this->target_0 < 0)
            ret -= 0.5 * (this->inv0 +spell.delta0 - this->target_0);
        if (this->inv1 +spell.delta1 - this->target_1 < 0)
            ret -= 2 * (this->inv1 +spell.delta1 - this->target_1);
        if (this->inv2 + spell.delta2 - this->target_2 < 0)
            ret -= 3 * (this->inv2 +spell.delta2 - this->target_2);
        if (this->inv3 + spell.delta3 - this->target_3 < 0)
            ret -= 4 * (this->inv3 + spell.delta3 - this->target_3);
        return (ret);
    }
};

int set_recipe_target_index(recettes tab_recette[5], inv inv);
bool is_action_id(int id, recettes tab_recette[5]);
int learn_spell(spells tome_spell[6], int turn, inv inv);
int cast_learnt_spell(inv inv, spells tab_spell[46]);

int main()
{
    // game loop
    spells tab_spell[46]; // 4 + 42 max
    recettes tab_recettes[5];
    spells tome_spell[6];
    inv inv;
    int index_cast;
    int index_learn;
    int index_brew;
    int op_score;
    bool change_in_op_score;
    int turn(-1);
    while (1) 
    {
        index_cast = 0;
        index_brew = 0;
        index_learn = 0;
        inv.num_spell = 0;
        turn++;


        int actionCount; // the number of spells and recipes in play
        cin >> actionCount; cin.ignore();
        // on boucle sur chaque recette demandée
        for (int i = 0; i < actionCount; i++) 
        {
            int actionId; // the unique ID of this spell or recipe
            string actionType; // in the first league: BREW; later: CAST, OPPONENT_CAST, LEARN, BREW
            int delta0; // tier-0 ingredient change
            int delta1; // tier-1 ingredient change
            int delta2; // tier-2 ingredient change
            int delta3; // tier-3 ingredient change
            int price; // the price in rupees if this is a potion
            int tomeIndex; // in the first two leagues: always 0; later: the index in the tome if this is a tome spell, equal to the read-ahead tax
            int taxCount; // in the first two leagues: always 0; later: the amount of taxed tier-0 ingredients you gain from learning this spell
            bool castable; // in the first league: always 0; later: 1 if this is a castable player spell
            bool repeatable; // for the first two leagues: always 0; later: 1 if this is a repeatable player spell
            cin >> actionId >> actionType >> delta0 >> delta1 >> delta2 >> delta3 >> price >> tomeIndex >> taxCount >> castable >> repeatable; cin.ignore();
            if (actionType == "BREW")
            {
                tab_recettes[index_brew].actionId = actionId;
                //cerr << index_recettes << " " << tab_recettes[index_recettes].actionId << std::endl;
                tab_recettes[index_brew].delta0 = delta0;
                tab_recettes[index_brew].delta1 = delta1;
                tab_recettes[index_brew].delta2 = delta2;
                tab_recettes[index_brew].delta3 = delta3;
                tab_recettes[index_brew].price = price;
                index_brew++;
            }
            if (actionType == "LEARN")
            {
                tome_spell[index_learn].actionId = actionId;
                tome_spell[index_learn].delta0 = delta0;
                tome_spell[index_learn].delta1 = delta1;
                tome_spell[index_learn].delta2 = delta2;
                tome_spell[index_learn].delta3 = delta3;
                tome_spell[index_learn].repeatable = repeatable;
                tome_spell[index_learn].tax_count = taxCount;
                tome_spell[index_learn].tome_index = tomeIndex;
                index_learn++;
            }
            if (actionType == "CAST")
            {
                tab_spell[index_cast].actionId = actionId;
                tab_spell[index_cast].delta0 = delta0;
                tab_spell[index_cast].delta1 = delta1;
                tab_spell[index_cast].delta2 = delta2;
                tab_spell[index_cast].delta3 = delta3;
                tab_spell[index_cast].castable = castable;
                index_cast++;
                inv.num_spell++;
            }
        }
        for (int i = 0; i < 2; i++) 
        {
            int inv0; // tier-0 ingredients in inventory
            int inv1;
            int inv2;
            int inv3;
            int score; // amount of rupees
            cin >> inv0 >> inv1 >> inv2 >> inv3 >> score; cin.ignore();
            //cerr << inv0 << endl;
            if (i == 0)
            {
                inv.inv0 = inv0;
                inv.inv1 = inv1;
                inv.inv2 = inv2;
                inv.inv3 = inv3;
            }
            if (i == 1)
            {
                if (op_score == score)
                    change_in_op_score = false;
                else
                    change_in_op_score = true;
                op_score = score;
            }
        }

        cerr << "num_spell: " << inv.num_spell << endl;
        // choix de la target: ne recalculer que si:
        //  unset OU nouvelle recette dispo (car adversaire ou moi en a utilisé une)
        // ou après un learnt spell ?
        int comb_target = inv.recipe_target_actionid;
        if (inv.recipe_target_actionid == -1 || change_in_op_score == true)
        {
            inv.recipe_target_index = set_recipe_target_index(tab_recettes, inv);
            inv.recipe_target_actionid = tab_recettes[inv.recipe_target_index].actionId;
            inv.target_0 = -tab_recettes[inv.recipe_target_index].delta0;
            inv.target_1 = -tab_recettes[inv.recipe_target_index].delta1;
            inv.target_2 = -tab_recettes[inv.recipe_target_index].delta2;
            inv.target_3 = -tab_recettes[inv.recipe_target_index].delta3;
            cerr << "target action id is " << inv.recipe_target_actionid << endl;
        }
        if (inv.can_do_target_recipe())
        {
            cout << "BREW " << inv.recipe_target_actionid << endl;
            inv.recipe_target_actionid = -1;
            continue;
        }

        if (inv.comb2_is_on)
        {
            if (is_action_id(comb_target, tab_recettes) == false)
            {
                cerr << "comb2 aborted" << endl;
                inv.comb2_is_on = false;
            }
            else
            {
                cout << "CAST " << inv.next_comb_action << endl;
                inv.comb2_is_on = false;
                continue;
            }
        }
        if (inv.comb3_is_on)
        {
            if (is_action_id(comb_target, tab_recettes) == false)
            {
                cerr << "comb3 aborted" << endl;
                inv.comb3_is_on = false;
            }
            else if (inv.next_comb_action != -1)
            {
                cout << "CAST " << inv.next_comb_action << endl;
                inv.next_comb_action = -1;
                continue;
            }
            else
            {
                cout << "CAST " << inv.next_next_comb_action << endl;
                inv.comb3_is_on = false;
                continue;
            }
        }

        // determine whether we learn a spell
        int learn_id = -1;
        if (turn < 20)
            learn_id = learn_spell(tome_spell, turn, inv);
        if (learn_id != -1)
        {
            cerr << "LEARN" << learn_id << endl;
            cout << "LEARN " << learn_id << endl;
            continue;
        }

        // try to find a good 3comb
        int min_proj_3 = inv.effort();
        cerr << "proj effort " << min_proj_3 << endl;
        int first_action_3(-1);
        int second_action_3(-1);
        int third_action_3(-1);
        for (int i = 0; i < inv.num_spell; i++)
        {
            if (tab_spell[i].castable == false || inv.can_do_spell(tab_spell[i]) == false || inv.enough_room_for_spell(tab_spell[i]) == false)
                continue;
            for (int j = 0; j < inv.num_spell; j++)
            {
                if (i == j || tab_spell[j].castable == false)
                    continue;
                if (i < 4 && j < 4)
                    continue;
                class inv new_inv(inv);
                new_inv.inv0 = inv.inv0 + tab_spell[i].delta0;
                new_inv.inv1 = inv.inv1 + tab_spell[i].delta1;
                new_inv.inv2 = inv.inv2 + tab_spell[i].delta2;
                new_inv.inv3 = inv.inv3 + tab_spell[i].delta3;
                if (new_inv.way_enough_room_for_spell(tab_spell[j]) == false || new_inv.can_do_spell(tab_spell[j]) == false)
                    continue;
                for (int k = 0; k < inv.num_spell; k++)
                {
                    if (k == i || k == j || tab_spell[k].castable == false)
                        continue;
                    if (i < 4 && j < 4 && k < 4)
                        continue;
                    class inv new_new_inv(new_inv);
                    new_new_inv.inv0 = new_inv.inv0 + tab_spell[j].delta0;
                    new_new_inv.inv1 = new_inv.inv1 + tab_spell[j].delta1;
                    new_new_inv.inv2 = new_inv.inv2 + tab_spell[j].delta2;
                    new_new_inv.inv3 = new_inv.inv3 + tab_spell[j].delta3;
                    if (new_new_inv.way_enough_room_for_spell(tab_spell[k]) == false || new_new_inv.can_do_spell(tab_spell[k]) == false)
                        continue;
                    if (new_new_inv.projected_effort(tab_spell[k]) < min_proj_3)
                    {
                        //cerr << "here" << endl;
                        min_proj_3 = new_new_inv.projected_effort(tab_spell[k]);
                        first_action_3 = tab_spell[i].actionId;
                        second_action_3 = tab_spell[j].actionId;
                        third_action_3 = tab_spell[k].actionId;
                    }
                }
            }
        }

        // try to find a good 2comb
        int min_proj_2 = inv.effort();
        int first_action_2 = -1;
        int second_action_2 = -1;
        for (int i = 0; i < inv.num_spell; i++)
        {
            if (tab_spell[i].castable == false || inv.can_do_spell(tab_spell[i]) == false || inv.enough_room_for_spell(tab_spell[i]) == false)
                continue;
            for (int j = 0; j < inv.num_spell; j++)
            {
                if (i == j || tab_spell[j].castable == false)
                    continue;
                if (i < 4 && j < 4)
                    continue;
                class inv new_inv(inv);
                new_inv.inv0 = inv.inv0 + tab_spell[i].delta0;
                new_inv.inv1 = inv.inv1 + tab_spell[i].delta1;
                new_inv.inv2 = inv.inv2 + tab_spell[i].delta2;
                new_inv.inv3 = inv.inv3 + tab_spell[i].delta3;
                if (new_inv.way_enough_room_for_spell(tab_spell[j]) == false || new_inv.can_do_spell(tab_spell[j]) == false)
                    continue;
                //cerr << "i: " << i << " j: " << j << " " << new_inv.projected_effort(tab_spell[j]) << endl;
                if (new_inv.projected_effort(tab_spell[j]) < min_proj_2)
                {
                    //cerr << "here" << endl;
                    min_proj_2 = new_inv.projected_effort(tab_spell[j]);
                    first_action_2 = tab_spell[i].actionId;
                    second_action_2 = tab_spell[j].actionId;
                }
            }
        }
        /*
        // fill target_tier
        // if "direct" way(s) to do tier, do it
        bool casted(false);
        for(int i = 0;i < inv.num_spell;i++)
        {
            if (tab_spell[i].neg_deltas() == 0 &&
            tab_spell[i].castable &&
            inv.enough_room_for_spell(tab_spell[i]) && 
            ((inv.stop_index() == 1 && tab_spell[i].delta0 > 0)
            || (inv.stop_index() == 2 && tab_spell[i].delta1 > 0)
            || (inv.stop_index() == 3 && tab_spell[i].delta2 > 0)
            || (inv.stop_index() == 4 && tab_spell[i].delta3 > 0)))
            {
                cerr << "direct" << endl;
                cout << "CAST " << tab_spell[i].actionId << endl;
                casted = true;
                break;
            }
        }
        if (casted == true)
            continue;
        */

        // chose from learn spells: spell that minimizes effort to get to target recipe
        // if min_effort < current_effort, do that spell, else go to classic spells
        int first_action = -1;
        int min_proj = inv.effort();
        for (int i = 0; i < inv.num_spell; i++)
        {
            if (tab_spell[i].castable && inv.can_do_spell(tab_spell[i]) &&
            inv.way_enough_room_for_spell(tab_spell[i])
            && inv.projected_effort(tab_spell[i]) < min_proj)
            {
                //cerr << inv.projected_effort(tab_spell[i]) << endl;
                min_proj = inv.projected_effort(tab_spell[i]);
                first_action = tab_spell[i].actionId;
            }
        }

        cerr << "min projs " << min_proj << " " << min_proj_2 << " " << min_proj_3 << endl;
        
        if (first_action_3 != -1 && min_proj_3 + 2 < min_proj_2 && min_proj_3 + 4 < min_proj)
        {
            cerr << "going for comb3" << endl;
            cout << "CAST " << first_action_3 << endl;
            inv.comb3_is_on = true;
            inv.next_comb_action = second_action_3;
            inv.next_next_comb_action = third_action_3;
            continue;
        }
        if (first_action_2 != -1 && min_proj_2 + 2 < min_proj)
        {
            cerr << "going for comb2" << endl;
            cout << "CAST " << first_action_2 << endl;
            inv.comb2_is_on = true;
            inv.next_comb_action = second_action_2;
            continue;
        }
        if (first_action != -1 && min_proj + 2 < inv.effort())
        {
            cerr << "going for single" << endl;
            cout << "CAST " << first_action << endl;
            continue;
        }
        int num_castable = 0;
        for (int i = 0; i < inv.num_spell; i++)
        {
            if (tab_spell[i].castable)
                num_castable++;
        }
        cerr << num_castable << endl;
        if ((inv.num_spell > 9 && num_castable - 2 <= inv.num_spell / 2)
        || (inv.num_spell <= 9 && num_castable - 1 <= inv.num_spell / 2))
        {
            cerr << "forced rest" << endl;
            cout << "REST" << endl;
            continue;
        }
        
        cerr << "going classic" << endl;
        // chose spell (from initial spells)
        int i = inv.start_index();
        while (i < inv.stop_index())
        {
            if (tab_spell[i].castable)
            {
                if (inv.can_do_spell(tab_spell[i])
                && inv.enough_room_for_spell(tab_spell[i]))
                {
                    cout << "CAST " << tab_spell[i].actionId << endl;
                    break;
                }
            }
            i++;
        }
        if (i == inv.stop_index())
        {
            // if spell[0] is not exhausted, it may be worth to use it before resting:
            if (tab_spell[0].castable && inv.enough_room_for_spell(tab_spell[0]) &&
            inv.tiers_still_needed() == true)
                cout << "CAST " << tab_spell[0].actionId << endl;
            else
                cout << "REST" << endl;
        }
    }
}

// returns tome_spell id if a spell is worth learning. returns -1 else
int learn_spell(spells tome_spell[6], int turn, inv inv)
{
    int max_score(-100);
    int index_max;
    int score(0);
    int limit;

    if (inv.num_spell < 6)
        limit = -1;
    else if (inv.num_spell < 8)
        limit = 0;
    else if (inv.num_spell < 10)
        limit = 1;
    else if (inv.num_spell < 12)
        limit = 2;
    else if (inv.num_spell < 14)
        limit = 3;
    else
        limit = 10;

    for(int i = 0;i<6;i++)
    {
        //cerr << tome_spell[i].ascend_or_flat() << endl;
        score = tome_spell[i].score() + (tome_spell[i].tax_count - i) / 2;
        if (tome_spell[i].neg_deltas() == 0 && tome_spell[i].delta0 > 2)
            score += 2;
        //cerr << tome_spell[i].score() << " " << score << endl;
        if (score > max_score && i <= inv.inv0)
        {
            max_score = score;
            index_max = i;
        }
    }
    //cerr << "max index" << index_max << endl;
    if (max_score > limit)
        return tome_spell[index_max].actionId;
    else
        return -1;
}

int cast_learnt_spell(inv inv, spells tab_spell[46])
{
    int min_proj_effort(100);
    int spell_id;
    //cerr << "effort " << inv.effort() << endl;
    for (int i = 0; i < inv.num_spell; i++)
    {
        if (tab_spell[i].castable && inv.can_do_spell(tab_spell[i]) &&
        inv.way_enough_room_for_spell(tab_spell[i]) && 
        inv.not_too_much_of_one(tab_spell[i])
        && inv.projected_effort(tab_spell[i]) < min_proj_effort)
        {
            //cerr << inv.projected_effort(tab_spell[i]) << endl;
            min_proj_effort = inv.projected_effort(tab_spell[i]);
            spell_id = tab_spell[i].actionId;
        }
    }
    if (min_proj_effort < inv.effort())
    {
        cerr << "will use learnt spell" << endl;
        return spell_id;
    }
    else
    {
        cerr << "will NOT use learnt spell" << endl;
        return -1;
    }

}

// classic mix between price and effort
int set_recipe_target_index(recettes tab_recettes[5], inv inv)
{
    int max_score(-10);
    int recette_index;
    for(int i = 0; i<5; i++)
    {
        //cerr << "i " << i << " effort: " << inv.effort(tab_recettes[i]) << endl;
        if (tab_recettes[i].price - 1.6 * inv.effort(tab_recettes[i]) > max_score)
        {
            max_score = tab_recettes[i].price - 1.6 * inv.effort(tab_recettes[i]);
            recette_index = i;
        }
    }
    //cerr << "max score: " << max_score << endl;
    return recette_index;
}


/*
// go for the fastest recipe
int set_recipe_target_index(recettes tab_recettes[5], inv inv)
{
    int min_effort = 100;
    int price = 0;
    int recette_index;
    for(int i = 0; i<5; i++)
    {
        cerr << "i " << i << " effort: " << tab_recettes[i].effort(inv) << endl;
        if (tab_recettes[i].effort(inv) < min_effort
        || (tab_recettes[i].effort(inv) == min_effort && tab_recettes[i].price > price))
        {
            min_effort = tab_recettes[i].effort(inv);
            price = tab_recettes[i].price;
            recette_index = i;
        }
    }
    cerr << "min_effort: " << min_effort << endl;
    return recette_index;
}
*/

bool is_action_id(int id, recettes tab_recette[5])
{
    int i = 0;
    while (i < 5)
    {
        if (tab_recette[i].actionId == id)
            return true;
        i++;
    }
    return false;
}