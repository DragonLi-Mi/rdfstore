
PREFIX b:<http://yago-knowledge.org/resource/>
PREFIX xsd:<http://www.w3.org/2001/XMLSchema#>
PREFIX rdfs:<http://www.w3.org/2000/01/rdf-schema#> 

select  ?name1
where {

   b:yagoTheme_yagoWordnetIds b:hasNumber  ?name1.


}
