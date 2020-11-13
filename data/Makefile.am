gsettings_schemas_in_in = \
	apps.gooroom-indicator-applet.gschema.xml.in.in
gsettings_schemas_in = $(gsettings_schemas_in_in:.xml.in.in=.xml.in)
gsettings_SCHEMAS = $(gsettings_schemas_in:.xml.in=.xml)

@INTLTOOL_XML_NOMERGE_RULE@

%.gschema.xml.in: %.gschema.xml.in.in Makefile
	$(AM_V_GEN) $(SED) -e 's^\@GETTEXT_PACKAGE\@^$(GETTEXT_PACKAGE)^g' < $< > $@

@GSETTINGS_RULES@

%.panel-applet: %.panel-applet.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

EXTRA_DIST = \
	kr.gooroom.IndicatorApplets.panel-applet.in.in \
	$(gsettings_schemas_in_in)

CLEANFILES = \
	$(applet_DATA) \
	$(applet_DATA).in \
	$(gsettings_schemas_in) \
	$(gsettings_SCHEMAS) \
	*.gschema.valid
