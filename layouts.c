/* Layouts */
static void bstack(Monitor *m);
static void bstackhoriz(Monitor *m);
static void centeredmaster(Monitor *m);
static void centeredfloatingmaster(Monitor *m);
static void deck(Monitor *m);
static void dwindle(Monitor *m);
static void fibonacci(Monitor *m, int s);
static void spiral(Monitor *m);
static void tile(Monitor *m);
static void tatami(Monitor *m);
static void horizgrid(Monitor *m);
static void grid(Monitor *m);

void
bstack(Monitor *m)
{
	unsigned int i, n, w, mh, mx, tx;
	float mfacts = 0, sfacts = 0;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (n < m->nmaster)
			mfacts += c->cfact;
		else
			sfacts += c->cfact;
	}
	if (n == 0)
		return;
	if(n == 1){
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
		return;
	}

	if (n > m->nmaster)
		mh = m->nmaster ? m->wh * m->mfact : 0;
	else
		mh = m->wh;
	for (i = 0, mx = tx = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			w = (m->ww - mx) * (c->cfact / mfacts);
			resize(c, m->wx + mx, m->wy, w - (2*c->bw), mh - 2*c->bw, 0);
			if(mx + WIDTH(c) < m->mw)
				mx += WIDTH(c);
			mfacts -= c->cfact;
		} else {
			w = (m->ww - tx) * (c->cfact / sfacts);
			resize(c, m->wx + tx, m->wy + mh, w - (2*c->bw), m->wh - mh - 2*(c->bw), 0);
			if(tx + WIDTH(c) < m->mw)
				tx += WIDTH(c);
			sfacts -= c->cfact;
		}
}

void
bstackhoriz(Monitor *m)
{
	unsigned int i, n, h, mw, mh, my, ty;
	float mfacts = 0, sfacts = 0;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (n < m->nmaster)
			mfacts += c->cfact;
		else
			sfacts += c->cfact;
	}
	if (n == 0)
		return;
	if(n == 1){
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
		return;
	}

	if (n > m->nmaster)
		mh = m->nmaster ? m->wh * m->mfact : 0;
	else
		mh = m->wh;
	mw = m->ww;

	for (i = ty = 0, my = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (mh - my) * (c->cfact / mfacts);
			resize(c, m->wx, m->wy + my, mw - 2*c->bw, h - 2*c->bw, 0);
			if(my + HEIGHT(c) < m->mh)
				my += HEIGHT(c);
			mfacts -= c->cfact;
		} else {
			h = (m->wh - mh - ty) * (c->cfact / sfacts);
			resize(c, m->wx, m->wy + mh + ty, mw - 2*c->bw, h - (2*c->bw), 0);
			if(ty + HEIGHT(c) < m->mh)
				ty += HEIGHT(c);
			sfacts -= c->cfact;
		}
}

void
centeredmaster(Monitor *m)
{
	unsigned int i, n, h, mw, mx, my, oty, ety, tw;
	float mfacts = 0, lfacts = 0, rfacts = 0;
	Client *c;

	/* count number of clients in the selected monitor */
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (n < m->nmaster)
			mfacts += c->cfact;
		else if ((n - m->nmaster) % 2) 
			lfacts += c->cfact;
		else
			rfacts += c->cfact;
	}
	if (n == 0)
		return;
	if(n == 1){
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
		return;
	}

	/* initialize areas */
	mw = m->ww;
	mx = 0;
	my = 0;
	tw = mw;

	if (n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		mw = m->nmaster ? m->ww * m->mfact : 0;
		tw = m->ww - mw;

		if (n - m->nmaster > 1) {
			/* only one client */
			mx = (m->ww - mw) / 2;
			tw = (m->ww - mw) / 2;
		}
	}

	oty = 0;
	ety = 0;
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
	if (i < m->nmaster) {
		/* nmaster clients are stacked vertically, in the center
		 * of the screen */
		h = (m->wh - my) * (c->cfact / mfacts);
		resize(c, m->wx + mx, m->wy + my, mw - 2*c->bw,
		      h - 2*c->bw, 0);
		if(my + HEIGHT(c) < m->mh)
			my += HEIGHT(c);
		mfacts -= c->cfact;
	} else {
		/* stack clients are stacked vertically */
		if ((i - m->nmaster) % 2) {
			h = (m->wh - ety) * (c->cfact / lfacts);
			if(m->nmaster == 0)
				resize(c, m->wx, m->wy + ety, tw - 2*c->bw,
			       h - 2*c->bw, 0);
			else
				resize(c, m->wx, m->wy + ety, tw - 2*c->bw,
			       h - 2*c->bw, 0);
			if(ety + HEIGHT(c) < m->mh)
				ety += HEIGHT(c);
			lfacts -= c->cfact;
		} else {
			h = (m->wh - oty) * (c->cfact / rfacts);
			resize(c, m->wx + mx + mw, m->wy + oty,
			       tw - 2*c->bw, h - 2*c->bw, 0);
			if(oty + HEIGHT(c) < m->mh)
				oty += HEIGHT(c);
			rfacts -= c->cfact;
		}
	}
}

void
centeredfloatingmaster(Monitor *m)
{
	unsigned int i, n, w, mh, mw, mx, mxo, my, myo, tx;
	float mfacts = 0, sfacts = 0;
	Client *c;

	/* count number of clients in the selected monitor */
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (n < m->nmaster)
			mfacts += c->cfact;
		else
			sfacts += c->cfact;
	}
	if (n == 0)
		return;
	if(n == 1){
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
		return;
	}

	/* initialize nmaster area */
	if (n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		if (m->ww > m->wh) {
			mw = m->nmaster ? m->ww * m->mfact : 0;
			mh = m->nmaster ? m->wh * 0.9 : 0;
		} else {
			mh = m->nmaster ? m->wh * m->mfact : 0;
			mw = m->nmaster ? m->ww * 0.9 : 0;
		}
		mx = mxo = (m->ww - mw) / 2;
		my = myo = (m->wh - mh) / 2;
	} else {
		/* go fullscreen if all clients are in the master area */
		mh = m->wh;
		mw = m->ww;
		mx = mxo = 0;
		my = myo = 0;
	}

	for(i = tx = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
	if (i < m->nmaster) {
		/* nmaster clients are stacked horizontally, in the center
		 * of the screen */
		w = (mw + mxo - mx) * (c->cfact / mfacts);
		resize(c, m->wx + mx, m->wy + my, w - 2*c->bw,
		       mh - 2*c->bw, 0);
		if(mx + WIDTH(c) < m->mw)
			mx += WIDTH(c);
		mfacts -= c->cfact; 
	} else {
		/* stack clients are stacked horizontally */
		w = (m->ww - tx) * (c->cfact / sfacts);
		resize(c, m->wx + tx, m->wy, w - 2*c->bw,
		       m->wh - 2*c->bw, 0);
		if(tx + WIDTH(c) < m->mw)
			tx += WIDTH(c);
		sfacts -= c->cfact; 
	}
}

static void
deck(Monitor *m)
{
	unsigned int i, n, h, mw, my;
	float mfacts = 0;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (n < m->nmaster)
			mfacts += c->cfact;
	}
	if(n == 0)
		return;

	if(n > m->nmaster) {
		mw = m->nmaster ? m->ww * m->mfact : 0;
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n - m->nmaster);
	}
	else
		mw = m->ww;
	for(i = my = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if(i < m->nmaster) {
			h = (m->wh - my) * (c->cfact / mfacts);
			resize(c, m->wx, m->wy + my, mw - (2*c->bw), h - (2*c->bw), False);
			if (my + HEIGHT(c) < m->wh)
				my += HEIGHT(c);
			mfacts -= c->cfact;
		}
		else
			resize(c, m->wx + mw, m->wy, m->ww - mw - (2*c->bw), m->wh - (2*c->bw), False);
}

void
fibonacci(Monitor *mon, int s)
{
	unsigned int i, n, nx, ny, nw, nh;
	Client *c;

	for(n = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next), n++);
	if(n == 0)
		return;
	
	nx = mon->wx;
	ny = 0;
	nw = mon->ww;
	nh = mon->wh;
	
	for(i = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next)) {
		if((i % 2 && nh / 2 > 2 * c->bw)
		   || (!(i % 2) && nw / 2 > 2 * c->bw)) {
			if(i < n - 1) {
				if(i % 2)
					nh /= 2;
				else
					nw /= 2;
				if((i % 4) == 2 && !s)
					nx += nw;
				else if((i % 4) == 3 && !s)
					ny += nh;
			}
			if((i % 4) == 0) {
				if(s)
					ny += nh;
				else
					ny -= nh;
			}
			else if((i % 4) == 1)
				nx += nw;
			else if((i % 4) == 2)
				ny += nh;
			else if((i % 4) == 3) {
				if(s)
					nx += nw;
				else
					nx -= nw;
			}
			if(i == 0)
			{
				if(n != 1)
					nw = mon->ww * mon->mfact;
				ny = mon->wy;
			}
			else if(i == 1)
				nw = mon->ww - nw;
			i++;
		}
		resize(c, nx, ny, nw - 2 * c->bw, nh - 2 * c->bw, False);
	}
}

void
dwindle(Monitor *mon)
{
	fibonacci(mon, 1);
}

void
spiral(Monitor *mon)
{
	fibonacci(mon, 0);
}

void
tile(Monitor *m)
{
	unsigned int i, n, h, mw, my, ty;
	float mfacts = 0, sfacts = 0;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (n < m->nmaster)
			mfacts += c->cfact;
		else
			sfacts += c->cfact;
	}
	if (n == 0)
		return;

	if (n > m->nmaster)
		mw = m->nmaster ? m->ww * m->mfact : 0;
	else
		mw = m->ww;
	for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my) * (c->cfact / mfacts);
			resize(c, m->wx, m->wy + my, mw - (2*c->bw), h - (2*c->bw), 0);
			if (my + HEIGHT(c) < m->wh)
				my += HEIGHT(c);
     mfacts -= c->cfact;
		} else {
			h = (m->wh - ty) * (c->cfact / sfacts);
			resize(c, m->wx + mw, m->wy + ty, m->ww - mw - (2*c->bw), h - (2*c->bw), 0);
			if (ty + HEIGHT(c) < m->wh)
				ty += HEIGHT(c);
     sfacts -= c->cfact;
		}
}

void
grid(Monitor *m)
{
	unsigned int i, n, cx, cy, cw, ch, aw, ah, cols, rows;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next))
		n++;

	/* grid dimensions */
	for(rows = 0; rows <= n/2; rows++)
		if(rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = m->wh / (rows ? rows : 1);
	cw = m->ww / (cols ? cols : 1);
	for(i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		cx = m->wx + (i / rows) * cw;
		cy = m->wy + (i % rows) * ch;
		/* adjust height/width of last row/column's windows */
		ah = ((i + 1) % rows == 0) ? m->wh - ch * rows : 0;
		aw = (i >= rows * (cols - 1)) ? m->ww - cw * cols : 0;
		resize(c, cx, cy, cw - 2 * c->bw + aw, ch - 2 * c->bw + ah, False);
		i++;
	}
}

void
horizgrid(Monitor *m)
{
	Client *c;
	unsigned int n, i;
	int w = 0;
	int ntop, nbottom = 0;

	/* Count windows */
	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);

	if(n == 0)
		return;
	else if(n == 1) { /* Just fill the whole screen */
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, m->ww - (2*c->bw), m->wh - (2*c->bw), False);
	} else if(n == 2) { /* Split vertically */
		w = m->ww / 2;
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, w - (2*c->bw), m->wh - (2*c->bw), False);
		c = nexttiled(c->next);
		resize(c, m->wx + w, m->wy, w - (2*c->bw), m->wh - (2*c->bw), False);
	} else {
		ntop = n / 2;
		nbottom = n - ntop;
		for(i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
			if(i < ntop)
				resize(c, m->wx + i * m->ww / ntop, m->wy, m->ww / ntop - (2*c->bw), m->wh / 2 - (2*c->bw), False);
			else
				resize(c, m->wx + (i - ntop) * m->ww / nbottom, m->wy + m->wh / 2, m->ww / nbottom - (2*c->bw), m->wh / 2 - (2*c->bw), False);
		}
	}
}

void 
tatami(Monitor *m)
{
	unsigned int i, n, nx, ny, nw, nh,
				 mats, tc,
				 tnx, tny, tnw, tnh;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), ++n);
	if(n == 0)
		return;
	
	nx = m->wx;
	ny = 0;
	nw = m->ww;
	nh = m->wh;
	
	c = nexttiled(m->clients);
	
	if(n != 1)  nw = m->ww * m->mfact;
				ny = m->wy;
				
	resize(c, nx, ny, nw - 2 * c->bw, nh - 2 * c->bw, False);
	
	c = nexttiled(c->next);
	
	nx += nw;
	nw = m->ww - nw;
	
	if(n>1)
	{
	
	tc = n-1;
	mats = tc/5;
	
	nh/=(mats + (tc % 5 > 0));
	
	for(i = 0; c && (i < (tc % 5)); c = nexttiled(c->next))
	{
		tnw=nw;
		tnx=nx;
		tnh=nh;
		tny=ny;
		switch(tc - (mats*5))
				{
					case 1://fill
						break;
					case 2://up and down
						if((i % 5) == 0) //up
						tnh/=2;
						else if((i % 5) == 1) //down
						{
							tnh/=2;
							tny += nh/2;
						}
						break;
					case 3://bottom, up-left and up-right
						if((i % 5) == 0) //up-left
						{
						tnw = nw/2;
						tnh = (2*nh)/3;
						}
						else if((i % 5) == 1)//up-right
						{
							tnx += nw/2;
							tnw = nw/2;
							tnh = (2*nh)/3;
						}
						else if((i % 5) == 2)//bottom
						{
							tnh = nh/3;
							tny += (2*nh)/3;	
						}
						break;
					case 4://bottom, left, right and top
						if((i % 5) == 0) //top
						{
							tnh = (nh)/4;
						}
						else if((i % 5) == 1)//left
						{
							tnw = nw/2;
							tny += nh/4;
							tnh = (nh)/2;
						}
						else if((i % 5) == 2)//right
						{
							tnx += nw/2;
							tnw = nw/2;
							tny += nh/4;
							tnh = (nh)/2;
						}
						else if((i % 5) == 3)//bottom
						{
							tny += (3*nh)/4;
							tnh = (nh)/4;
						}
						break;
				}
		++i;
		resize(c, tnx, tny, tnw - 2 * c->bw, tnh - 2 * c->bw, False);
	}
	
	++mats;
	
	for(i = 0; c && (mats>0); c = nexttiled(c->next)) {

			if((i%5)==0)
			{
			--mats;
			if(((tc % 5) > 0)||(i>=5))
			ny+=nh;
			}
			
			tnw=nw;
			tnx=nx;
			tnh=nh;
			tny=ny;
			

			switch(i % 5)
			{
				case 0: //top-left-vert
					tnw = (nw)/3;
					tnh = (nh*2)/3;
					break;
				case 1: //top-right-hor
					tnx += (nw)/3;
					tnw = (nw*2)/3;
					tnh = (nh)/3;
					break;
				case 2: //center
					tnx += (nw)/3;
					tnw = (nw)/3;
					tny += (nh)/3;
					tnh = (nh)/3;
					break;
				case 3: //bottom-right-vert
					tnx += (nw*2)/3;
					tnw = (nw)/3;
					tny += (nh)/3;
					tnh = (nh*2)/3;
					break;
				case 4: //(oldest) bottom-left-hor
					tnw = (2*nw)/3;
					tny += (2*nh)/3;
					tnh = (nh)/3;
					break;
				default:
					break;
			}
			
			++i;
			//i%=5;
		resize(c, tnx, tny, tnw - 2 * c->bw, tnh - 2 * c->bw, False);
		}
	}
}
