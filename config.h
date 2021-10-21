/** CONSTANTS ***/

/* universal modifier*/
#define MODKEY Mod4Mask

/* programs */
#define TERMINAL "alacritty"
#define BROWSER "qutebrowser"

/* classes for user-defined rules */
#define FLOATCLASS "floating"
#define FLOATCENTERCLASS "floatcenter"

/* appearance */
static unsigned int borderpx  = 3;    /* border pixel of windows */
static unsigned int snap      = 32;   /* snap pixel */
static int swallowfloating    = 1;    /* 1 means swallow floating windows by default */
static int showbar            = 1;    /* 0 means no bar */
static int topbar             = 1;    /* 0 means bottom bar */
static int attachbelow        = 1;    /* 1 means attach at the end */
static int resizehints        = 1;    /* 1 means respect size hints in tiled resizals */
static int nmaster            = 1;    /* default number of clients in master area */
static float mfact            = 0.84; /* factor of master area size [0.05..0.95] */
static char *fonts[]          = { "SourceCodePro:size=12" };
/* static char *fonts[]          = { "monospace:size=14" }; */
static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[]     = "#bbbbbb";
static char selfgcolor[]      = "#eeeeee";
static char selbordercolor[]  = "#770000";
static char selbgcolor[]      = "#005577";
static char *colors[][3] = {
  /*               fg           bg           border   */
  [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
  [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;

const char spcmd1n[] = "spd1";
const char spcmd2n[] = "spd2";
const char spcmd3n[] = "spd3";
const char *spcmd1[] = { "tabbed", "-n", spcmd1n, "-c", "alacritty", "--embed" };
const char *spcmd2[] = { "tabbed", "-n", spcmd2n, "-c", "alacritty", "--embed" };
const char *spcmd3[] = { "alacritty", "--class", spcmd3n, "-e", "coins", "-c"};
static Sp scratchpads[] = {
  /* name   command */
  { spcmd1n, spcmd1 },
  { spcmd2n, spcmd2 },
  { spcmd3n, spcmd3 },
};

static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* class instance title tagsmask isfloating isterminal noswallow monitor useCustomGeometry x,y,w,h floatcenter*/

  /* enable swallow feature for terminal programs */
  { "Termite",   NULL,    NULL,  0, 0, 1, 0, -1, 0,0,0,0,0, 0},
  { "St"     ,   NULL,    NULL,  0, 0, 1, 0, -1, 0,0,0,0,0, 0},
  { "Alacritty", NULL,    NULL,  0, 0, 1, 0, -1, 0,0,0,0,0, 0},

  { NULL, "Termite",      NULL,  0, 0, 1, 0, -1, 0,0,0,0,0, 0},
  { NULL, "St"     ,      NULL,  0, 0, 1, 0, -1, 0,0,0,0,0, 0},
  { NULL, "Alacritty",    NULL,  0, 0, 1, 0, -1, 0,0,0,0,0, 0},

  /* some special rules for terminal programms*/
  { "termfloatcenter", NULL, NULL, 0, 1, 1, 0, -1, 1,-1,-1,90,79, 1},
  { "Ranger",          NULL, NULL, 0, 1, 0, 0, -1, 1,10,10,80,80, 0},
  { NULL, "termfloatcenter", NULL, 0, 1, 1, 0, -1, 1,-1,-1,90,79, 1},
  { NULL,          "Ranger", NULL, 0, 1, 0, 0, -1, 1,10,10,80,80, 0},

  /* set dimensions and positions of some programs */
  { "Pavucontrol",   NULL, NULL, 0, 1, 0, 0, -1, 1,25,25,50,50 ,0},
  { "Zathura",       NULL, NULL, 0, 1, 0, 0, -1, 1,10,10,80,80, 0},
  { "mpv",           NULL, NULL, 0, 1, 0, 0, -1, 1,10,10,80,80, 0},

  /* float center some programs */
  { "Sxiv",          NULL, NULL, 0, 1, 0, 1, -1, 0,0,0,0,0, 1},
  { "R_x11",         NULL, NULL, 0, 1, 0, 1, -1, 0,0,0,0,0, 1},
  { "GNU Octave",    NULL, NULL, 0, 1, 0, 1, -1, 0,0,0,0,0, 1},

  /* rules for scratchpads */
  { spcmd1n, NULL,  NULL,  SPTAG(0), 1, 1, 0, -1, 1,-1,-1,90,79,  1},
  { spcmd2n, NULL,  NULL,  SPTAG(1), 1, 1, 0, -1, 1,0.5,85,99,14, 0},
  { spcmd3n, NULL,  NULL,  SPTAG(2), 1, 1, 0, -1, 1,35,60,64,39,  0},
  { NULL, spcmd1n,  NULL,  SPTAG(0), 1, 1, 0, -1, 1,-1,-1,90,79,  1},
  { NULL, spcmd2n,  NULL,  SPTAG(1), 1, 1, 0, -1, 1,0.5,85,99,14, 0},
  { NULL, spcmd3n,  NULL,  SPTAG(2), 1, 1, 0, -1, 1,35,60,64,39,  0},
};

/* layout(s) */
#define FORCE_VSPLIT 1 /* nrowgrid layout: force two clients to always split vertically */

#include "layouts.c"
#include "tagall.c"
#include "maximize.c"

static const Layout
  layout_bstack = { "TTT", bstack },
  layout_tile = { "[]=", tile },
  layout_spiral = { "[@]", spiral },
  layout_dwindle = { "[\\]", dwindle },
  layout_deck = { "[D]", deck },
  layout_monocle = { "[M]", monocle },
  layout_centeredmaster = { "|M|", centeredmaster },
  layout_centeredfloatingmaster = { ">M>", centeredfloatingmaster },
  layout_floating = { "><>", NULL },
  layout_grid = { "HHH", grid },
  layout_horizgrid = { "###", horizgrid },
  layout_tatami = { "|+|",  tatami },
  layout_bstackhoriz = { "===",  bstackhoriz };

static const Layout layouts[] = {
  layout_bstack,
  layout_bstackhoriz,
  layout_tile,
  layout_tatami,
  layout_dwindle,
  layout_spiral,
  layout_monocle,
  layout_deck,
  layout_centeredmaster,
  layout_centeredfloatingmaster,
  layout_grid,
  layout_horizgrid,
  layout_floating,
};

/* key definitions */
#define AltMask Mod1Mask
#define TAGKEYS(KEY,TAG) \
  { MODKEY,                       KEY, view,       {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY, toggleview, {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY, tag,        {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY, toggletag,  {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
  { MOD,  XK_j, ACTION##stack,  {.i = INC(+1) } }, \
  { MOD,  XK_k, ACTION##stack,  {.i = INC(-1) } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* Xresources preferences to load at startup */
ResourcePref resources[] = {
  { "color0",          STRING,   &normbordercolor },
  { "color8",          STRING,   &selbordercolor },
  { "color0",          STRING,   &normbgcolor },
  { "color4",          STRING,   &normfgcolor },
  { "color0",          STRING,   &selfgcolor },
  { "color4",          STRING,   &selbgcolor },
  { "borderpx",        INTEGER,  &borderpx },
  { "snap",            INTEGER,  &snap },
  { "showbar",         INTEGER,  &showbar },
  { "topbar",          INTEGER,  &topbar },
  { "nmaster",         INTEGER,  &nmaster },
  { "resizehints",     INTEGER,  &resizehints },
  { "mfact",           FLOAT,    &mfact },
  { "swallowfloating", INTEGER,  &swallowfloating },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
  TAGKEYS(XK_1, 0)
  TAGKEYS(XK_2, 1)
  TAGKEYS(XK_3, 2)
  TAGKEYS(XK_4, 3)
  TAGKEYS(XK_5, 4)
  TAGKEYS(XK_6, 5)
  TAGKEYS(XK_7, 6)
  TAGKEYS(XK_8, 7)
  TAGKEYS(XK_9, 8)

  /* default bindings */
  { MODKEY,             XK_0,   view,       {.ui = ~0 } },
  { MODKEY|ShiftMask,   XK_0,   tag,        {.ui = ~0 } },
  { MODKEY,             XK_Tab, view,       {0} },
  { MODKEY,             XK_q,   killclient, {0} },
  { MODKEY|ShiftMask,   XK_q,   killunsel,  {0} },
  { MODKEY|ControlMask, XK_q,   quit,       {0} },

  /* keybindings for keyboards with volume keys */
  { 0,          XF86XK_AudioMute,  spawn,  SHCMD("volumectl toogle-mute") },
  { 0,   XF86XK_AudioRaiseVolume,  spawn,  SHCMD("volumectl inc 5") },
  { 0,   XF86XK_AudioLowerVolume,  spawn,  SHCMD("volumectl dec 5") },

  /* keybindings for keyboards without volume keys */
  { MODKEY,             XK_m,      spawn,  SHCMD("volumectl toggle-mute") },
  { MODKEY|ShiftMask,   XK_minus,  spawn,  SHCMD("volumectl dec 5") },
  { MODKEY|ShiftMask,   XK_equal,  spawn,  SHCMD("volumectl inc 5") },

  /* keybindings for keyboards with screen brightness keys */
  { 0, XF86XK_MonBrightnessUp,     spawn,  SHCMD("light -A 5") },
  { 0, XF86XK_MonBrightnessDown,   spawn,  SHCMD("light -U 5") },

  /* keybindings for keyboards without screen brightness keys */
  { MODKEY, XK_minus,                 spawn, SHCMD("light -U 5") },
  { MODKEY, XK_equal,                 spawn, SHCMD("light -A 5") },

  /* tagall keybindings  */
  { MODKEY|ShiftMask,     XK_F1,      tagall,  {.v = "1"} },
  { MODKEY|ShiftMask,     XK_F2,      tagall,  {.v = "2"} },
  { MODKEY|ShiftMask,     XK_F3,      tagall,  {.v = "3"} },
  { MODKEY|ShiftMask,     XK_F4,      tagall,  {.v = "4"} },
  { MODKEY|ShiftMask,     XK_F5,      tagall,  {.v = "5"} },
  { MODKEY|ShiftMask,     XK_F6,      tagall,  {.v = "6"} },
  { MODKEY|ShiftMask,     XK_F7,      tagall,  {.v = "7"} },
  { MODKEY|ShiftMask,     XK_F8,      tagall,  {.v = "8"} },
  { MODKEY|ShiftMask,     XK_F9,      tagall,  {.v = "9"} },

  /* toggle attach below */
  { MODKEY|ShiftMask,     XK_Tab,     toggleAttachBelow, {0} },

  /* keybingins for resizing windows in stack area */
  { MODKEY|AltMask,     XK_i,      setcfact,       {.f = -0.25} },
  { MODKEY|AltMask,     XK_o,      setcfact,       {.f = +0.25} },
  { MODKEY|AltMask,     XK_p,      setcfact,       {.f =  0.00} },

  /* commands */
  { MODKEY,             XK_v,  spawn,  SHCMD("nvim-gui") },
  { MODKEY|ShiftMask,   XK_v,  spawn,  SHCMD("nvim-gui --center") },
  { MODKEY,             XK_e,  spawn,  SHCMD("alacritty -e neomutt") },
  { MODKEY,        XK_Return,  spawn,  SHCMD("alacritty") },
  { MODKEY,             XK_w,  spawn,  SHCMD(BROWSER) },

  /* menus */
  { MODKEY,             XK_d,  spawn,  SHCMD("menu-applications") },
  { MODKEY,             XK_c,  spawn,  SHCMD("menu-clipboard") },
  { MODKEY,             XK_p,  spawn,  SHCMD("menu-password") },
  { MODKEY|ControlMask, XK_l,  spawn,  SHCMD("menu-lorem") },
  { MODKEY|ShiftMask,   XK_p,  spawn,  SHCMD("menu-otp") },

  /* lock screen */
  { MODKEY|ShiftMask,   XK_l,  spawn,  SHCMD("screen-locker") },

  /* control screen temperature color */
  { MODKEY,           XK_F1,  spawn,  SHCMD("screen-color dec 500") },
  { MODKEY,           XK_F2,  spawn,  SHCMD("screen-color inc 500") },

  /* layouts */
  { MODKEY,             XK_t,  setlayout,  {.v = &layout_tile } },
  { MODKEY|ShiftMask,   XK_t,  setlayout,  {.v = &layout_tatami } },
  { MODKEY,             XK_y,  setlayout,  {.v = &layout_dwindle } },
  { MODKEY|ShiftMask,   XK_y,  setlayout,  {.v = &layout_spiral } },
  { MODKEY,             XK_u,  setlayout,  {.v = &layout_deck } },
  { MODKEY|ShiftMask,   XK_u,  setlayout,  {.v = &layout_bstackhoriz } },
  { MODKEY,             XK_i,  setlayout,  {.v = &layout_centeredmaster } },
  { MODKEY|ShiftMask,   XK_i,  setlayout,  {.v = &layout_centeredfloatingmaster } },
  { MODKEY,             XK_g,  setlayout,  {.v = &layout_grid } },
  { MODKEY|ShiftMask,   XK_g,  setlayout,  {.v = &layout_horizgrid } },
  { MODKEY,             XK_s,  setlayout,  {.v = &layout_bstack } },
  { MODKEY,             XK_f,  setlayout,  {.v = &layout_monocle } },
  { MODKEY|ShiftMask,   XK_f,  setlayout,  {.v = &layout_floating } },

  /* change the layout of a single window */
  { MODKEY|ShiftMask,   XK_s,     togglesticky,   {0} },
  { MODKEY|ShiftMask,   XK_space, togglefloating, {0} },

  /* toggle whether focused client is in master area */
  { MODKEY,             XK_space, zoom,           {0} },

  /* change the number of clients in master area */
  { MODKEY|AltMask,     XK_minus,  incnmaster, {.i = -1 } },
  { MODKEY|AltMask,     XK_equal,  incnmaster, {.i = +1 } },

  /* navigate through clients  */
  { MODKEY,  XK_j,     focusstack,  {.i = INC(+1) } },
  { MODKEY,  XK_k,     focusstack,  {.i = INC(-1) } },
  { MODKEY,  XK_Right, focusstack,  {.i = INC(+1) } },
  { MODKEY,  XK_Left,  focusstack,  {.i = INC(-1) } },

  /* swap client up/down */
  { MODKEY|ShiftMask, XK_j,           pushstack,      {.i = INC(+1) } },
  { MODKEY|ShiftMask, XK_k,           pushstack,      {.i = INC(-1) } },

  /* resize master area */
  { MODKEY,           XK_h,           setmfact,       {.f = -0.05 } },
  { MODKEY,           XK_l,           setmfact,       {.f = +0.05 } },

  /* navigate through tags */
  { MODKEY,           XK_comma,       shiftview,      {.i = -1 } },
  { MODKEY,           XK_period,      shiftview,      {.i = +1 } },
  { MODKEY|ShiftMask, XK_Left,        shiftview,      {.i = -1 } },
  { MODKEY|ShiftMask, XK_Right,       shiftview,      {.i = +1 } },

  /* shift focused window to next/previous tag */
  { MODKEY|ShiftMask, XK_comma,       shifttag,       { .i = -1 } },
  { MODKEY|ShiftMask, XK_period,      shifttag,       { .i = +1 } },

  /* scratchpads */
  { MODKEY|ShiftMask, XK_Return,      togglescratch,  {.ui = 0} },
  { MODKEY,           XK_apostrophe,  togglescratch,  {.ui = 1} },
  { MODKEY|ShiftMask, XK_apostrophe,  togglescratch,  {.ui = 2} },

  /* keybindings for resizing floating windows */
  { MODKEY|ControlMask, XK_n,         togglehorizontalmax, {0} },
  { MODKEY|ControlMask, XK_m,         toggleverticalmax,   {0} },
  { MODKEY|ControlMask, XK_space,     togglemaximize,      {0} },

  /* keybindings for moving floating windows */
  { MODKEY|AltMask,  XK_Up,    movethrow, {.ui = DIR_N  }},
  { MODKEY|AltMask,  XK_Down,  movethrow, {.ui = DIR_S  }},
  { MODKEY|AltMask,  XK_Left,  movethrow, {.ui = DIR_W  }},
  { MODKEY|AltMask,  XK_Right, movethrow, {.ui = DIR_E  }},
  { MODKEY|AltMask,  XK_k,     movethrow, {.ui = DIR_N  }},
  { MODKEY|AltMask,  XK_j,     movethrow, {.ui = DIR_S  }},
  { MODKEY|AltMask,  XK_h,     movethrow, {.ui = DIR_W  }},
  { MODKEY|AltMask,  XK_l,     movethrow, {.ui = DIR_E  }},
  { MODKEY|AltMask,  XK_space, movethrow, {.ui = DIR_C  }},

  /* bar */
  { MODKEY,           XK_b,           togglebar,      {0} },

  /* arrow keys */

  /* page keys */
  { MODKEY,           XK_Page_Up,   shiftview,  { .i = -1 } },
  { MODKEY,           XK_Page_Down, shiftview,  { .i = +1 } },
  /* { MODKEY|ShiftMask,   XK_Page_Up,   shifttag,   { .i = -1 } }, */
  /* { MODKEY|ShiftMask,   XK_Page_Down, shifttag,   { .i = +1 } }, */

  /* keybindings for taking screenshots */
  { 0,           XK_Print, spawn, SHCMD("screen-wizard -s") },
  { ShiftMask,   XK_Print, spawn, SHCMD("screen-wizard") },
  { MODKEY,      XK_F11,   spawn, SHCMD("record-webcam") },

  /* other stuff */
  { 0,  XF86XK_WWW,          spawn, SHCMD(BROWSER) },
  { 0,  XF86XK_DOS,          spawn, SHCMD(TERMINAL) },
  { 0,  XF86XK_AudioMicMute, spawn, SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click          event mask  button    function        argument */
#ifndef __OpenBSD__
  { ClkWinTitle,    0,          Button2,  zoom,         {0} },
  { ClkStatusText,  0,          Button1,  sigdwmblocks, {.i = 1} },
  { ClkStatusText,  0,          Button2,  sigdwmblocks, {.i = 2} },
  { ClkStatusText,  0,          Button3,  sigdwmblocks, {.i = 3} },
  { ClkStatusText,  ShiftMask,  Button1,  sigdwmblocks, {.i = 6} },
#endif
  { ClkClientWin,   MODKEY,     Button1,  movemouse,    {0} },
  { ClkClientWin,   MODKEY,     Button3,  resizemouse,  {0} },
  { ClkTagBar,      0,          Button1,  view,         {0} },
  { ClkTagBar,      0,          Button3,  toggleview,   {0} },
  { ClkTagBar,      MODKEY,     Button1,  tag,          {0} },
  { ClkTagBar,      MODKEY,     Button3,  toggletag,    {0} },
  { ClkTagBar,      0,          Button4,  shiftview,    {.i = -1} },
  { ClkTagBar,      0,          Button5,  shiftview,    {.i = 1} },
  { ClkRootWin,     0,          Button2,  togglebar,    {0} },
};
