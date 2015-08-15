from django.conf.urls import patterns, url

from comp import views

urlpatterns = patterns("",
	url (r"_login/", views.comp_login, name = "login"),
	url (r"_files/", views.comp_files, name = "files"),
	url (r"_signup/", views.comp_signup, name = "signup"),
	url (r"_logout/", views.comp_logout, name = "logout"),
	url (r"_new/", views.comp_new, name = "new"),
	url (r"_old/", views.comp_old, name = "old"),
	url (r"_debug/", views.comp_debug, name = "debug"),
)