package provide ade::ui 1.0

#      Author: Manolis M. Tsangaris  -- mmt@athenads.com
#   Copyright: ATHENA DESIGN SYSTEMS, 2004


proc css_setup {} {
    option add *Menu*font            "helvetica 10 bold"
    option add *Menubutton*font      "helvetica 10 bold"
    option add *Button*font          "helvetica 10 bold"
    option add *LPopupText*Text*font "courier 10"
    option add *LMenu*font           "helvetica 10 bold"
    option add *LLinearToolBar*font  "helvetica 10 bold"

    option add *Filter*Label.background gray80
    option add *Filter*Frame.background gray75
    option add *filter*font    "helvetica 8"

    option add *ZUI.LConsole*font  "courier 8"
    option add *ZConsole*font  "courier 8"

    option add *MSHViewer*font "helvetica 8"

    option add *CPanel*font "helvetica 8 bold"
    option add *LSTree*font "helvetica 8"

}
