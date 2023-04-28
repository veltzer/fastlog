<%!
    import pydmt.helpers.project
    import pydmt.helpers.misc
    import pydmt.helpers.signature
    import config.project
    import config.personal
%><!DOCTYPE html>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<title>${pydmt.helpers.project.get_name()}</title>
		<link rel="shortcut icon" href="../static/favicon.ico"/>
		<meta name="Description" content="${config.project.description_long}"/>
		<meta name="Keywords" content="${config.personal.fullname}, ${config.personal.slug}, ${pydmt.helpers.project.get_name()}, ${', '.join(config.project.keywords)}"/>
	</head>
	<body>
		<h1>Welcome to the <i>${pydmt.helpers.project.get_name()}</i> web site</h1>

		<p>current version is ${pydmt.helpers.misc.get_version_str()}</p>

		<h2>Fastlog</h2>
		<p>
		TBD
		</p>
		<p>
			Copyright ${config.personal.fullname} © ${pydmt.helpers.signature.get_copyright_years_long()}
			<a href="mailto:${config.personal.email}">${config.personal.email}</a>
		</p>
	</body>
</html>
