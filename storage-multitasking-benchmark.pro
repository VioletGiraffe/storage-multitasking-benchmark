TEMPLATE = subdirs

SUBDIRS += app cpputils core cpp-template-utils

core.depends = cpputils

app.depends = core