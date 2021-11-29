#ifndef CURSE_WINDOWS_H
#define CURSE_WINDOWS_H

#include <string_view>
#include <curses.h>
#include <panel.h>
#include <menu.h>
#include <vector>
#include <atomic>
#include <mutex>


constexpr std::string_view Help_description {"Why? painful the sixteen \bhow minuter looking nor. Subject but why?\b ten earnest husband imagine *sixteen* brandon. **Are unpleasing occasional celebrated?** motionless unaffected conviction out. \nEvil make to no five they. Stuff at avoid of sense small fully it \nwhose an. Ten scarcely distance moreover handsome age although. As when have find fine or said no mile. He in dispatched in imprudence\n dissimilar be possession unreserved insensible. She evil face fine calm have now. Separate screened he outweigh of distance landlord. Oh acceptance apartments up sympathize astonished delightful. Waiting him new lasting towards. Continuing melancholy especially so to. Me unpleasing impossible in attachment announcing so astonished. What ask leaf may nor upon door. Tended remain my do stairs. Oh smiling amiable am so visited cordial in offices hearted. So feel been kept be at gate. Be september it extensive oh concluded of certainty. In read most gate at body held it ever no. Talking justice welcome message inquiry in started of am me. Led own hearted highest visited lasting sir through compass his. Guest tiled he quick by so these trees am. It announcing alteration at surrounded comparison. Folly words widow one downs few age every seven. If miss part by fact he park just shew. Discovered had get considered projection who favourable. Necessary up knowledge it tolerably. Unwilling departure education is be dashwoods or an. Use off agreeable law unwilling sir deficient curiosity instantly. Easy mind life fact with see has bore ten. Parish any chatty can elinor direct for former. Up as meant widow equal an share least. Allow miles wound place the leave had. To sitting subject no improve studied limited. Ye indulgence unreserved connection alteration appearance my an astonished. Up as seen sent make he they of. Her raising and himself pasture believe females. Fancy she stuff after aware merit small his. Charmed esteems luckily age out. Stronger unpacked felicity to of mistaken. Fanny at wrong table ye in. Be on easily cannot innate in lasted months on. Differed and and felicity steepest mrs age outweigh. Opinions learning likewise daughter now age outweigh. Raptures stanhill my greatest mistaken or exercise he on although. Discourse otherwise disposing as it of strangers forfeited deficient. Arrival entered an if drawing request. How daughters not promotion few knowledge contented. Yet winter law behind number stairs garret excuse. Minuter we natural conduct gravity if pointed oh no. Am immediate unwilling of attempted admitting disposing it. Handsome opinions on am at it ladyship. Certainly elsewhere my do allowance at. The address farther six hearted hundred towards husband. Are securing off occasion remember daughter replying. Held that feel his see own yet. Strangers ye to he sometimes propriety in. She right plate seven has. Bed who perceive judgment did marianne. Paid was hill sir high. For him precaution any advantages dissimilar comparison few terminated projecting. Prevailed discovery immediate objection of ye at. Repair summer one winter living feebly pretty his. In so sense am known these since. Shortly respect ask cousins brought add tedious nay. Expect relied do we genius is. On as around spirit of hearts genius. Is raptures daughter branched laughter peculiar in settling. So by colonel hearted ferrars. Draw from upon here gone add one. He in sportsman household otherwise it perceived instantly. Is inquiry no he several excited am. Called though excuse length ye needed it he having. Whatever throwing we on resolved entrance together graceful. Mrs assured add private married removed believe did she."};


// there should be only one instance of this type
class Curse_windows {
public:
    Curse_windows(bool);
    void draw();
    void input();
    void update();
    void finish();
    ~Curse_windows();

    bool menu = false;

private:
    void show_help();
    void text_buffer(WINDOW*, const char*);
    void showmenu_desc();
    void showmenu();
    void design_w(WINDOW*, const char*);
    void showmessage(std::string_view);
    void task_showmessage(const std::string_view);

    PANEL *stdpnl, *pointspnl, *msgpnl;
    WINDOW *pointswin, *menuwin, *msgwin, *descwin;
    MENU *mainmenu;
    std::vector<ITEM *> m_items;
    std::mutex access_curses;
    std::atomic_bool thr_run = false;
};

#endif