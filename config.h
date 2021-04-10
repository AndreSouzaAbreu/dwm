/* Constants */
#define MODKEY Mod4Mask
#define TERMINAL "termite"
#define TERMCLASS "Termite"
#define BROWSER "qutebrowser"

/* appearance */
static unsigned int borderpx  = 3;  /* border pixel of windows */
static unsigned int snap      = 32; /* snap pixel */
static int swallowfloating    = 0;  /* 1 means swallow floating windows by default */
static int showbar            = 1;  /* 0 means no bar */
static int topbar             = 1;  /* 0 means bottom bar */
static char *fonts[]          = { "monospace:size=14" };
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
const char *spcmd1[] = {"st", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
  /* name    cmd  */
  {"spterm", spcmd1},
  {"spcalc", spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* class      instance    title           tags mask  isfloating   isterminal  noswallow  monitor */
  { "Gimp",     NULL,       NULL,           1 << 8,     0,           0,         0,         -1 },
  { TERMCLASS,  NULL,       NULL,           0,          0,           1,         0,         -1 },
  { NULL,       NULL,       "Event Tester", 0,          0,           0,         1,         -1 },
  { NULL,      "spterm",    NULL,           SPTAG(0),   1,           1,         0,         -1 },
  { NULL,      "spcalc",    NULL,           SPTAG(1),   1,           1,         0,         -1 },
};

/* layout(s) */
static const float default_mfact = 0.80;
static float mfact = default_mfact;  /* factor of master area size [0.05..0.95] */
static int nmaster = 1;     /* number of clients in master area */
static int resizehints = 1; /* 1 means respect size hints in tiled resizals */
static int attachbelow = 1; /* 1 means attach at the end */
#define FORCE_VSPLIT 1      /* nrowgrid layout: force two clients to always split vertically */

#include "layouts.c"
#include "tagall.c"
#include "maximize.c"

static const Layout
  layout_bstack = { "TTT", bstack },
  layout_tile = { "[]=", tile },
  layout_spiral = { "[@]", spiral },
  layout_dwindle = { "[\\]",dwindle },
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

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

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
  { MODKEY,           XK_0,   view,       {.ui = ~0 } },
  { MODKEY|ShiftMask, XK_0,   tag,        {.ui = ~0 } },
  { MODKEY,           XK_Tab, view,       {0} },
  { MODKEY,           XK_q,   killclient, {0} },
  { MODKEY|ShiftMask, XK_q,   quit,       {0} },

  /* keybindings for keyboards with volume keys */
  { 0,          XF86XK_AudioMute,  spawn,  SHCMD("volumectl toogle-mute") },
  { 0,   XF86XK_AudioRaiseVolume,  spawn,  SHCMD("volumectl inc 5") },
  { 0,   XF86XK_AudioLowerVolume,  spawn,  SHCMD("volumectl dec 5") },

  /* keybindings for keyboards without volume keys */
  { MODKEY,             XK_m,      spawn,  SHCMD("volumectl toggle-mute") },
  { MODKEY|ShiftMask,   XK_minus,  spawn,  SHCMD("volumectl dec 5") },
  { MODKEY|ShiftMask,   XK_equal,  spawn,  SHCMD("volumectl inc 5") },

  /* keybindings for keyboards with screen brightness keys */
  { 0,      XF86XK_MonBrightnessUp,   spawn, SHCMD("light -A 5") },
  { 0,      XF86XK_MonBrightnessDown, spawn, SHCMD("light -U 5") },

  /* keybindings for keyboards without screen brightness keys */
  { MODKEY, XK_minus,                 spawn, SHCMD("light -U 5") },
  { MODKEY, XK_equal,                 spawn, SHCMD("light -A 5") },

  /* tagall keybindings  */
  { MODKEY|ShiftMask,     XK_F1,      tagall,        {.v = "1"} },
  { MODKEY|ShiftMask,     XK_F2,      tagall,        {.v = "2"} },
  { MODKEY|ShiftMask,     XK_F3,      tagall,        {.v = "3"} },
  { MODKEY|ShiftMask,     XK_F4,      tagall,        {.v = "4"} },
  { MODKEY|ShiftMask,     XK_F5,      tagall,        {.v = "5"} },
  { MODKEY|ShiftMask,     XK_F6,      tagall,        {.v = "6"} },
  { MODKEY|ShiftMask,     XK_F7,      tagall,        {.v = "7"} },
  { MODKEY|ShiftMask,     XK_F8,      tagall,        {.v = "8"} },
  { MODKEY|ShiftMask,     XK_F9,      tagall,        {.v = "9"} },

  /* toggle attach below */
  { MODKEY|ShiftMask,     XK_Tab,     toggleAttachBelow, {0} },

  /* keybingins for resizing windows in stack area */
  { MODKEY|AltMask,     XK_l,      setcfact,       {.f = +0.25} },
  { MODKEY|AltMask,     XK_h,      setcfact,       {.f = -0.25} },
  { MODKEY|AltMask,     XK_n,      setcfact,       {.f =  0.00} },

  /* commands */
  { MODKEY,             XK_v,  spawn,  SHCMD(TERMINAL " -e vim") },
  { MODKEY,             XK_e,  spawn,  SHCMD(TERMINAL " -e neomutt") },
  { MODKEY,             XK_w,  spawn,  SHCMD(BROWSER) },
  { MODKEY,             XK_d,  spawn,  SHCMD("menu-applications") },
  { MODKEY,             XK_c,  spawn,  SHCMD("menu-clipboard") },
  { MODKEY|ControlMask, XK_l,  spawn,  SHCMD("menu-lorem") },
  { MODKEY|ShiftMask,   XK_l,  spawn,  SHCMD("screen-lock") },
  { MODKEY,             XK_p,  spawn,  SHCMD("menu-password") },
  { MODKEY|ShiftMask,   XK_p,  spawn,  SHCMD("menu-otp") },

  /* control screen temperature color */
  { MODKEY,           XK_F1,  spawn,  SHCMD("screen-color dec 500") },
  { MODKEY,           XK_F2,  spawn,  SHCMD("screen-color inc 500") },

  /* layouts */
  { MODKEY,             XK_t,  setlayout,  {.v = &layout_tile } },
  { MODKEY|ShiftMask,   XK_t,  setlayout,  {.v = &layout_tatami } },
  { MODKEY,             XK_y,  setlayout,  {.v = &layout_dwindle } },
  { MODKEY|ShiftMask,   XK_y,  setlayout,  {.v = &layout_spiral } },
  { MODKEY,             XK_u,  setlayout,  {.v = &layout_monocle } },
  { MODKEY|ShiftMask,   XK_u,  setlayout,  {.v = &layout_deck } },
  { MODKEY,             XK_i,  setlayout,  {.v = &layout_centeredmaster } },
  { MODKEY|ShiftMask,   XK_i,  setlayout,  {.v = &layout_centeredfloatingmaster } },
  { MODKEY,             XK_g,  setlayout,  {.v = &layout_grid } },
  { MODKEY|ShiftMask,   XK_g,  setlayout,  {.v = &layout_horizgrid } },
  { MODKEY,             XK_s,  setlayout,  {.v = &layout_bstack } },
  { MODKEY|ShiftMask,   XK_s,  setlayout,  {.v = &layout_bstackhoriz } },
  { MODKEY|ShiftMask,   XK_f,  setlayout,  {.v = &layout_floating } },

  /* change the layout of a single window */
  { MODKEY,             XK_f,     togglefullscr,  {0} },
  { MODKEY|ControlMask, XK_s,     togglesticky,   {0} },
  { MODKEY|ShiftMask,   XK_space, togglefloating, {0} },

  /* toggle whether focused client is in master area */
  { MODKEY,             XK_space, zoom,           {0} },

  /* change the number of clients in master area */
  { MODKEY|AltMask,    XK_minus,  incnmaster,     {.i = -1 } },
  { MODKEY|AltMask,    XK_equal,  incnmaster,     {.i = +1 } },

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
  { MODKEY,           XK_n,           setmfact,       {.f = default_mfact } },

  /* navigate through tags */
  { MODKEY,           XK_period,      shiftview,      {.i = +1 } },
  { MODKEY,           XK_comma,       shiftview,      {.i = -1 } },
  { MODKEY|ShiftMask, XK_Left,        shiftview,      {.i = -1 } },
  { MODKEY|ShiftMask, XK_Right,       shiftview,      {.i = +1 } },

  /* shift focused window to next/previous tag */
  { MODKEY|ShiftMask, XK_period,      shifttag,       { .i = +1 } },
  { MODKEY|ShiftMask, XK_comma,       shifttag,       { .i = -1 } },

  { MODKEY,           XK_Return,      spawn,          {.v = termcmd } },
  /* { MODKEY|ShiftMask, XK_Return,      togglescratch,  {.ui = 0} }, */
  /* { MODKEY,           XK_apostrophe,  togglescratch,  {.ui = 1} }, */

  /* keybindings for resizing floating windows */
  { MODKEY|ControlMask, XK_k,         togglehorizontalmax, {0} },
  { MODKEY|ControlMask, XK_j,         toggleverticalmax,   {0} },
  { MODKEY|ControlMask, XK_m,         togglemaximize,      {0} },

  /* bar */
  { MODKEY,           XK_b,           togglebar,      {0} },

  /* arrow keys */

  /* page keys */
  { MODKEY,           XK_Page_Up,   shiftview,  { .i = -1 } },
  { MODKEY,           XK_Page_Down, shiftview,  { .i = +1 } },
  /* { MODKEY|ShiftMask,   XK_Page_Up,   shifttag,   { .i = -1 } }, */
  /* { MODKEY|ShiftMask,   XK_Page_Down, shifttag,   { .i = +1 } }, */


  /* keybindings for taking screenshots */
  { 0,         XK_Print, spawn, SHCMD("flameshot gui") },
  { ShiftMask, XK_Print, spawn, SHCMD("flameshot full --path ~/pictures/screenshots") },

  /* other stuff */
  { MODKEY,    XK_F11,   spawn, SHCMD("record-webcam") },

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
