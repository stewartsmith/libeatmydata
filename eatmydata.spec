Name:           eatmydata
Version:        @VERSION@
Release:        @RELEASE@%{?dist}
Group:          Development/Tools
License:        GPLv3
Summary:        library and utilities designed to disable fsync and friends
BuildRoot:      %{_tmppath}/%{name}-%{version}-root
BuildRequires:  autoconf, automake, make
Source:         %{name}-%{version}.tar.gz
#
%description
This package contains a small LD_PRELOAD library (libeatmydata) and a couple of helper utilities designed to transparently disable fsync and friends (like open(O_SYNC)). This has two side-effects: making software that writes data safely to disk a lot quicker and making this software no longer crash safe..
#
%prep
%setup -q
%build
autoreconf --force --install
%configure
%{__make}
%install
rm -fr %{buildroot}
%makeinstall
%clean
rm -fr %{buildroot}
#
%files
%defattr(-,root,root)
%attr(755,-,-) %{_bindir}/%{name}
%{_libdir}/*
%{_libexecdir}/*
%doc README AUTHORS COPYING
#
%changelog
* Fri Mar 08 2013 Alexey Bychko <alexey.bychko@percona.com> - 0.1
- Version 0.1, initial package for RPM-based systems


