SUBDIR = dc dd expr hash mktemp sbase sed test tr xargs

CPPFLAGS += -D'__dead=' -D'QUAD_MAX=((quad_t)(-1))' -D'SIZE_T_MAX=((size_t)(-1))' -D'REG_BASIC=0'
LDADD += -lbsd

.MAKE.EXPORTED += CPPFLAGS LDADD

.include <bsd.subdir.mk>
