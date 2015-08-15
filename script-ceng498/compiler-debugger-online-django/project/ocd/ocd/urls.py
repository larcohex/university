from django.conf.urls import patterns, include, url
from django.contrib import admin
from comp import views
urlpatterns = patterns ("",
    # Examples:
    # url(r'^$', 'ocd.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),
	url (r"^$", views.index, name = "index"),
	url (r"^comp", include ("comp.urls")),
    url (r"^admin/", include (admin.site.urls)),
)